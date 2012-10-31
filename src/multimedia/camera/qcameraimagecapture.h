/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCAMERAIMAGECAPTURE_H
#define QCAMERAIMAGECAPTURE_H

#include <qmediaobject.h>
#include <qmediaencodersettings.h>
#include <qmediabindableinterface.h>
#include <qvideoframe.h>

#include <qmediaenumdebug.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QSize;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class QImageEncoderSettings;

class QCameraImageCapturePrivate;
class Q_MULTIMEDIA_EXPORT QCameraImageCapture : public QObject, public QMediaBindableInterface
{
    Q_OBJECT
    Q_INTERFACES(QMediaBindableInterface)
    Q_ENUMS(Error)
    Q_ENUMS(CaptureDestination)
    Q_PROPERTY(bool readyForCapture READ isReadyForCapture NOTIFY readyForCaptureChanged)
public:
    enum Error
    {
        NoError,
        NotReadyError,
        ResourceError,
        OutOfSpaceError,
        NotSupportedFeatureError,
        FormatError
    };

    enum DriveMode
    {
        SingleImageCapture
    };

    enum CaptureDestination
    {
        CaptureToFile = 0x01,
        CaptureToBuffer = 0x02
    };
    Q_DECLARE_FLAGS(CaptureDestinations, CaptureDestination)

    QCameraImageCapture(QMediaObject *mediaObject, QObject *parent = 0);
    ~QCameraImageCapture();

    bool isAvailable() const;
    QMultimedia::AvailabilityStatus availability() const;

    QMediaObject *mediaObject() const;

    Error error() const;
    QString errorString() const;

    bool isReadyForCapture() const;

    QStringList supportedImageCodecs() const;
    QString imageCodecDescription(const QString &codecName) const;

    QList<QSize> supportedResolutions(const QImageEncoderSettings &settings = QImageEncoderSettings(),
                                      bool *continuous = 0) const;

    QImageEncoderSettings encodingSettings() const;
    void setEncodingSettings(const QImageEncoderSettings& settings);

    QList<QVideoFrame::PixelFormat> supportedBufferFormats() const;
    QVideoFrame::PixelFormat bufferFormat() const;
    void setBufferFormat(const QVideoFrame::PixelFormat format);

    bool isCaptureDestinationSupported(CaptureDestinations destination) const;
    CaptureDestinations captureDestination() const;
    void setCaptureDestination(CaptureDestinations destination);

public Q_SLOTS:
    int capture(const QString &location = QString());
    void cancelCapture();

Q_SIGNALS:
    void error(int id, QCameraImageCapture::Error error, const QString &errorString);

    void readyForCaptureChanged(bool);
    void bufferFormatChanged(QVideoFrame::PixelFormat);
    void captureDestinationChanged(QCameraImageCapture::CaptureDestinations);

    void imageExposed(int id);
    void imageCaptured(int id, const QImage &preview);
    void imageMetadataAvailable(int id, const QString &key, const QVariant &value);
    void imageAvailable(int id, const QVideoFrame &image);
    void imageSaved(int id, const QString &fileName);

protected:
    bool setMediaObject(QMediaObject *);

    QCameraImageCapturePrivate *d_ptr;
private:
    Q_DISABLE_COPY(QCameraImageCapture)
    Q_DECLARE_PRIVATE(QCameraImageCapture)
    Q_PRIVATE_SLOT(d_func(), void _q_error(int, int, const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_readyChanged(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_serviceDestroyed())
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QCameraImageCapture::CaptureDestinations)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QCameraImageCapture::Error)
Q_DECLARE_METATYPE(QCameraImageCapture::CaptureDestination)
Q_DECLARE_METATYPE(QCameraImageCapture::CaptureDestinations)

Q_MEDIA_ENUM_DEBUG(QCameraImageCapture, Error)
Q_MEDIA_ENUM_DEBUG(QCameraImageCapture, CaptureDestination)

QT_END_HEADER

#endif

