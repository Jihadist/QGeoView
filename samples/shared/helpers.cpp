/***************************************************************************
 * QGeoView is a Qt / C ++ widget for visualizing geographic data.
 * Copyright (C) 2018-2023 Andrey Yaroshenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see https://www.gnu.org/licenses.
 ****************************************************************************/

#include "helpers.h"

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QRandomGenerator>

namespace {
int randomInt()
{
    return static_cast<int>(QRandomGenerator::global()->generate());
}
}

QGV::GeoRect Helpers::randRect(QGVMap* geoMap, const QGV::GeoRect& targetArea, const QSizeF& size)
{
    const auto baseGeo = randPos(targetArea);
    const auto base = geoMap->getProjection()->geoToProj(baseGeo);
    return geoMap->getProjection()->projToGeo({ base, base + QPointF(size.width(), size.height()) });
}

QGV::GeoRect Helpers::randRect(QGVMap* geoMap, const QGV::GeoRect& targetArea, int baseSize)
{
    const auto size = randSize(baseSize);
    return randRect(geoMap, targetArea, size);
}

QGV::GeoPos Helpers::randPos(const QGV::GeoRect& targetArea)
{
    const double latRange = targetArea.latTop() - targetArea.latBottom();
    const double lonRange = targetArea.lonRigth() - targetArea.lonLeft();
    static const int range = 1000;

    return { targetArea.latBottom() + latRange * (randomInt() % range) / range,
             targetArea.lonLeft() + lonRange * (randomInt() % range) / range };
}

QSizeF Helpers::randSize(int baseSize)
{
    const int range = -baseSize / 2;

    return QSize(baseSize + (randomInt() % range), baseSize + (randomInt() % range));
}

void Helpers::setupCachedNetworkAccessManager(QObject* parent)
{
    QDir("cacheDir").removeRecursively();
    auto cache = new QNetworkDiskCache(parent);
    cache->setCacheDirectory("cacheDir");
    auto manager = new QNetworkAccessManager(parent);
    manager->setCache(cache);
    QGV::setNetworkManager(manager);
}
