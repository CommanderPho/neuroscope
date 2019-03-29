#include "videoplayer.h"

#include <QtWidgets>
//#include <QtMultimedia>
//#include <QtMultimediaWidgets>
#include <QVideoWidget>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>
#include <QMediaMetaData>

//currentMediaChanged(const QMediaContent &media)


VideoPlayer::VideoPlayer(QWidget *parent) : QWidget(parent)
{
    m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    QVideoWidget *videoWidget = new QVideoWidget;

    QAbstractButton *openButton = new QPushButton(tr("Open..."));
    connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    m_playButton = new QPushButton;
    m_playButton->setEnabled(false);
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked,
            this, &VideoPlayer::play);

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setRange(0, 0);

    connect(m_positionSlider, &QAbstractSlider::sliderMoved,
            this, &VideoPlayer::setPosition);


    m_positionLabel = new QLabel;
    m_positionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_showPositionInDataFileButton = new QPushButton(tr("Sync Data File"));
    m_showPositionInDataFileButton->setEnabled(false);
    //connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

    m_errorLabel = new QLabel;
    m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    //controlLayout->addWidget(openButton);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_positionSlider);

    QBoxLayout *secondaryControlsLayout = new QHBoxLayout;
    secondaryControlsLayout->setMargin(0);
    secondaryControlsLayout->addWidget(openButton);
    secondaryControlsLayout->addWidget(m_positionLabel);
    secondaryControlsLayout->addWidget(m_showPositionInDataFileButton);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);
    layout->addLayout(secondaryControlsLayout);
    layout->addWidget(m_errorLabel);

    setLayout(layout);

    m_mediaPlayer->setVideoOutput(videoWidget);
    connect(m_mediaPlayer, &QMediaPlayer::stateChanged, this, &VideoPlayer::mediaStateChanged);
    connect(m_mediaPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
    connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &VideoPlayer::handleError);
}

VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    QStringList supportedMimeTypes = m_mediaPlayer->supportedMimeTypes();
//    if (!supportedMimeTypes.isEmpty())
//        fileDialog.setMimeTypeFilters(supportedMimeTypes);


    // Set the default directory to look for the movie in
    const QString directoryURL = QDir::currentPath();
    //const QString directoryURL = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath());
    fileDialog.setDirectory(directoryURL);
    if (fileDialog.exec() == QDialog::Accepted)
        setUrl(fileDialog.selectedUrls().constFirst());
}

QUrl VideoPlayer::getUrl()
{
    return m_mediaPlayer->media().canonicalUrl();
}

void VideoPlayer::setUrl(const QUrl &url)
{
    qInfo(qUtf8Printable(url.toDisplayString(QUrl::PreferLocalFile)));
    m_errorLabel->setText(QString());
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    m_mediaPlayer->setMedia(url);
    m_playButton->setEnabled(true);
}

void VideoPlayer::play()
{
    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }
}

void VideoPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void VideoPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        this->videoFrameRate = m_mediaPlayer->metaData(QMediaMetaData::VideoFrameRate).value<qreal>();
    }
}

void VideoPlayer::getMetaData() {
    // Get the list of keys there is metadata available for
    QStringList metadatalist = m_mediaPlayer->availableMetaData();

    // Get the size of the list
    int list_size = metadatalist.size();

    //qDebug() << player->isMetaDataAvailable() << list_size;

    // Define variables to store metadata key and value
    QString metadata_key;
    QVariant var_data;

    for (int indx = 0; indx < list_size; indx++)
    {
        // Get the key from the list
        metadata_key = metadatalist.at(indx);

        // Get the value for the key
        var_data = m_mediaPlayer->metaData(metadata_key);

        qDebug() << metadata_key << var_data.toString();
    }



}

void VideoPlayer::positionChanged(qint64 position)
{
    m_positionSlider->setValue(position);
    this->updatePositionDurationLabel();
}

void VideoPlayer::durationChanged(qint64 duration)
{
    m_positionSlider->setRange(0, duration);
    this->updatePositionDurationLabel();
}

qint64 VideoPlayer::getDuration()
{
    return m_mediaPlayer->duration();
}

qint64 VideoPlayer::getPosition()
{
    return m_mediaPlayer->position();
}

qreal VideoPlayer::getFramesPerSecond() {
    return this->videoFrameRate;
}

void VideoPlayer::setPosition(int position)
{
    m_mediaPlayer->setPosition(position);
    this->updatePositionDurationLabel();
}


void VideoPlayer::updatePositionDurationLabel() {
    const qint64 currPosition = this->getPosition();
    const qint64 currDuration = this->getDuration();

    const QString strPosition = QString::number(currPosition); // actual conversion
    const QString strDuration = QString::number(currDuration); // actual conversion

    const QString finalOutputString = strPosition + " / " + strDuration;
    this->m_positionLabel->setText(finalOutputString);
}

void VideoPlayer::handleError()
{
    m_playButton->setEnabled(false);
    const QString errorString = m_mediaPlayer->errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
        message += " #" + QString::number(int(m_mediaPlayer->error()));
    else
        message += errorString;
    m_errorLabel->setText(message);
}
