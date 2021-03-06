/*=====================================================================

 QGroundControl Open Source Ground Control Station

 (c) 2009 - 2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

 This file is part of the QGROUNDCONTROL project

 QGROUNDCONTROL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QGROUNDCONTROL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

 ======================================================================*/

/// @file
///     @author Gus Grubba <mavlink@grubba.com>

#ifndef OfflineMapsManager_H
#define OfflineMapsManager_H

#include "QmlObjectListModel.h"
#include "QGCMapEngine.h"
#include "QGCMapTileSet.h"
#include "QGCMapEngineData.h"

class VGMapEngineManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int                  tileX0          READ    tileX0          NOTIFY tileX0Changed)
    Q_PROPERTY(int                  tileX1          READ    tileX1          NOTIFY tileX1Changed)
    Q_PROPERTY(int                  tileY0          READ    tileY0          NOTIFY tileY0Changed)
    Q_PROPERTY(int                  tileY1          READ    tileY1          NOTIFY tileY1Changed)
    Q_PROPERTY(quint64              tileCount       READ    tileCount       NOTIFY tileCountChanged)
    Q_PROPERTY(QString              tileCountStr    READ    tileCountStr    NOTIFY tileCountChanged)
    Q_PROPERTY(quint64              tileSize        READ    tileSize        NOTIFY tileSizeChanged)
    Q_PROPERTY(QString              tileSizeStr     READ    tileSizeStr     NOTIFY tileSizeChanged)
    Q_PROPERTY(QmlObjectListModel*  tileSets        READ    tileSets        NOTIFY tileSetsChanged)
    Q_PROPERTY(QStringList          mapList         READ    mapList         CONSTANT)
    Q_PROPERTY(QString              mapboxToken     READ    mapboxToken     WRITE   setMapboxToken  NOTIFY  mapboxTokenChanged)
    Q_PROPERTY(quint32              maxMemCache     READ    maxMemCache     WRITE   setMaxMemCache  NOTIFY  maxMemCacheChanged)
    Q_PROPERTY(quint32              maxDiskCache    READ    maxDiskCache    WRITE   setMaxDiskCache NOTIFY  maxDiskCacheChanged)
    Q_PROPERTY(QString              errorMessage    READ    errorMessage    NOTIFY  errorMessageChanged)
    //-- Disk Space in MB
    Q_PROPERTY(quint32              freeDiskSpace   READ    freeDiskSpace   NOTIFY  freeDiskSpaceChanged)
    Q_PROPERTY(quint32              diskSpace       READ    diskSpace       CONSTANT)
public:
    VGMapEngineManager();
    ~VGMapEngineManager();


    Q_INVOKABLE void                loadTileSets            ();
    Q_INVOKABLE void                updateForCurrentView    (double lon0, double lat0, double lon1, double lat1, int minZoom, int maxZoom, const QString &mapName);
    Q_INVOKABLE void                startDownload           (const QString& name, const QString &mapType);
    Q_INVOKABLE void                saveSetting             (const QString& key,  const QString &value);
    Q_INVOKABLE QString             loadSetting             (const QString& key,  const QString &defaultValue);
    Q_INVOKABLE void                deleteTileSet           (QGCCachedTileSet* tileSet);
    Q_INVOKABLE QString             getUniqueName           ();
    Q_INVOKABLE bool                findName                (const QString& name);

    int                             tileX0                  () { return _totalSet.tileX0; }
    int                             tileX1                  () { return _totalSet.tileX1; }
    int                             tileY0                  () { return _totalSet.tileY0; }
    int                             tileY1                  () { return _totalSet.tileY1; }
    quint64                         tileCount               () { return _totalSet.tileCount; }
    QString                         tileCountStr            ();
    quint64                         tileSize                () { return _totalSet.tileSize; }
    QString                         tileSizeStr             ();
    QStringList                     mapList                 ();
    QString                         mapboxToken             ();
    QmlObjectListModel*             tileSets                () { return &_tileSets; }
    quint32                         maxMemCache             ();
    quint32                         maxDiskCache            ();
    QString                         errorMessage            () { return _errorMessage; }
    quint64                         freeDiskSpace           () { return _freeDiskSpace; }
    quint64                         diskSpace               () { return _diskSpace; }

    void                            setMapboxToken          (QString token);
    void                            setMaxMemCache          (quint32 size);
    void                            setMaxDiskCache         (quint32 size);

    void                            setErrorMessage         (const QString &error) { _errorMessage = error; emit errorMessageChanged(); }

    // Override from QGCTool
    void setToolbox(/*QGCToolbox *toolbox*/);

    void setExportMaps(int tileSetID);
    void setImportMaps(const QString &setId, const QString &setTileName, const QList<QGCCacheTile*> &lstTiles);


signals:
    void tileX0Changed          ();
    void tileX1Changed          ();
    void tileY0Changed          ();
    void tileY1Changed          ();
    void tileCountChanged       ();
    void tileSizeChanged        ();
    void mapboxTokenChanged     ();
    void tileSetsChanged        ();
    void maxMemCacheChanged     ();
    void maxDiskCacheChanged    ();
    void errorMessageChanged    ();
    void freeDiskSpaceChanged   ();

    void tileSetExportedResult  (const QString &setID, const QString &setName, const QList<QGCCacheTile*> &tiles);

public slots:
    void taskError              (QGCMapTask::TaskType type, const QString &error);

private slots:
    void _tileSetSaved          (QGCCachedTileSet *set);
    void _tileSetFetched        (QGCCachedTileSet *tileSets);
    void _tileSetDeleted        (quint64 setID);
    void _updateTotals          (quint32 totaltiles, quint64 totalsize, quint32 defaulttiles, quint64 defaultsize);
    void _resetCompleted        ();

    void _tileSetExported       (const QString &setID, const QString &setName, const QList<QGCCacheTile*> &tiles);
    void _tileSetImportFinished (int setId);

private:
    void _updateDiskFreeSpace   ();

private:
    QGCTileSet  _totalSet;
    double      _topleftLat;
    double      _topleftLon;
    double      _bottomRightLat;
    double      _bottomRightLon;
    int         _minZoom;
    int         _maxZoom;
    quint64     _setID;
    quint32     _freeDiskSpace;
    quint32     _diskSpace;
    QmlObjectListModel _tileSets;
    QString     _errorMessage;
};

#endif
