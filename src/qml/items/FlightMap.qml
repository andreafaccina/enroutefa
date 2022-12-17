/***************************************************************************
 *   Copyright (C) 2019-2022 by Stefan Kebekus                             *
 *   stefan.kebekus@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

import QtLocation 5.15
import QtQuick 2.15
import QtQuick.Controls 2.15

import enroute 1.0

Map {
    id: flightMap

    /*! \brief Pixel per 10 kilometer

    This read-only propery is set to the number of screen pixel per ten
    kilometers on the map. It is updated whenever the zoom value changes. If the
    value cannot be determined for whatever reason, the property is set to zero.
    
    @warning The value is only a rough approximation and can be wrong at times.
    */
    property real pixelPer10km: 0.0
    
    /*! \brief Width of thick lines around airspaces, such as class D */
    property real airspaceLineWidth: 7.0

    /*
    * Handle changes in zoom level
    */
    
    onZoomLevelChanged: {
        var vec1 = flightMap.fromCoordinate(flightMap.center, false)
        var vec2 = flightMap.fromCoordinate(flightMap.center.atDistanceAndAzimuth(10000.0, 0.0), false)
        var dx = vec2.x - vec1.x
        var dy = vec2.y - vec1.y
        pixelPer10km = Math.sqrt(dx*dx+dy*dy);
    }
    
    onMapReadyChanged: {
        onZoomLevelChanged(zoomLevel)
    }

    maximumZoomLevel: 13.5
    minimumZoomLevel: 7.0001  // When setting 7 precisely, MapBox is looking for tiles of zoom 6, which we do not have…
    
    
    /*************************************
     * Aviation Data
     *************************************/
    
    DynamicParameter {
        type: "source"
        
        property string name: "aviationData"
        property string sourceType: "geojson"
        property string data: {
            if (global.dataManager().baseMapsRaster.hasFile)
                return global.geoMapProvider().emptyGeoJSON()
            return global.geoMapProvider().geoJSON
        }
    }

    /*************************************
     * Airspaces
     *************************************/



    // End of airspaces

    /*
     * Procedures
     */
    
    DynamicParameter {
        type: "layer"
        
        property string name: "PRC_DEP"
        property string layerType: "line"
        property string source: "aviationData"
        property var filter: ["all", ["==", ["get", "CAT"], "PRC"], ["==", ["get", "USE"], "DEP"]]
        property int minzoom: 10
    }
    DynamicParameter {
        type: "paint"
        property string layer: "PRC_DEP"
        property var lineColor: ["get", "GAC"]
        property real lineWidth: 3.0
        property var lineDasharray: [3.0, 3.0]
    }

    DynamicParameter {
        type: "layer"

        property string name: "PRC_ARR"
        property string layerType: "line"
        property string source: "aviationData"
        property var filter: ["all", ["==", ["get", "CAT"], "PRC"], ["==", ["get", "USE"], "ARR"]]
        property int minzoom: 10
    }
    DynamicParameter {
        type: "paint"
        property string layer: "PRC_ARR"
        property var lineColor: ["get", "GAC"]
        property real lineWidth: 3.0
        property var lineDasharray: [9.0, 3.0]
    }

    DynamicParameter {
        type: "layer"

        property string name: "PRC_OTH"
        property string layerType: "line"
        property string source: "aviationData"
        property var filter: ["all", ["==", ["get", "CAT"], "PRC"], ["!=", ["get", "USE"], "ARR"], ["!=", ["get", "USE"], "DEP"]]
        property int minzoom: 10
    }
    DynamicParameter {
        type: "paint"
        property string layer: "PRC_OTH"
        property var lineColor: ["get", "GAC"]
        property real lineWidth: 3.0
    }

    // We print PRC labels first and then label the traffic circuits. This way,
    // traffic circuit labels will be printed with higher priority
    DynamicParameter {
        type: "layer"

        property string name: "PRCLabels"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["all", ["==", ["get", "CAT"], "PRC"], ["!=", ["get", "USE"], "TFC"]]
        property int minzoom: 10
    }
    DynamicParameter {
        type: "layout"

        property string layer: "PRCLabels"
        property string symbolPlacement: "line"
        property var textField: ["get", "NAM"]
        property real textSize: 16
    }
    DynamicParameter {
        type: "paint"
        property string layer: "PRCLabels"
        property real textHaloWidth: 10
        property string textHaloColor: "white"
    }

    DynamicParameter {
        type: "layer"

        property string name: "TFCLabels"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["all", ["==", ["get", "CAT"], "PRC"], ["==", ["get", "USE"], "TFC"]]
        property int minzoom: 10
    }
    DynamicParameter {
        type: "layout"

        property string layer: "TFCLabels"
        property string symbolPlacement: "line"
        property var textField: ["get", "NAM"]
        property real textSize: 16
        property real symbolSpacing: 140
    }
    DynamicParameter {
        type: "paint"
        property string layer: "TFCLabels"
        property real textHaloWidth: 10
        property string textHaloColor: "white"
    }

    /*************************************
     * Waypoints
     *************************************/

    // Define the necessary images
    
    DynamicParameter {
        type: "image"
        
        property string name: "AD"
        property string sprite: ":flightMap/sprites/AD"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-GLD"
        property string sprite: ":flightMap/sprites/AD-GLD"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-GRASS"
        property string sprite: ":flightMap/sprites/AD-GRASS"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-INOP"
        property string sprite: ":flightMap/sprites/AD-INOP"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-MIL"
        property string sprite: ":flightMap/sprites/AD-MIL"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-MIL-GRASS"
        property string sprite: ":flightMap/sprites/AD-MIL-GRASS"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-MIL-PAVED"
        property string sprite: ":flightMap/sprites/AD-MIL-PAVED"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-PAVED"
        property string sprite: ":flightMap/sprites/AD-PAVED"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-UL"
        property string sprite: ":flightMap/sprites/AD-UL"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "AD-WATER"
        property string sprite: ":flightMap/sprites/AD-WATER"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "MRP"
        property string sprite: ":flightMap/sprites/MRP"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "NDB"
        property string sprite: ":flightMap/sprites/NDB"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "RP"
        property string sprite: ":flightMap/sprites/RP"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "VOR"
        property string sprite: ":flightMap/sprites/VOR"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "VOR-DME"
        property string sprite: ":flightMap/sprites/VORDME"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "VORTAC"
        property string sprite: ":flightMap/sprites/VORTAC"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "DVOR"
        property string sprite: ":flightMap/sprites/VOR"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "DVOR-DME"
        property string sprite: ":flightMap/sprites/VORDME"
    }
    DynamicParameter {
        type: "image"
        
        property string name: "DVORTAC"
        property string sprite: ":flightMap/sprites/VORTAC"
    }
    
    
    // NavAids - Text. Will be drawn with least priority
    
    DynamicParameter {
        type: "layer"
        
        property string name: "optionalText"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["==", ["get", "TYP"], "NAV"]
    }
    
    DynamicParameter {
        type: "layout"

        property string layer: "optionalText"
        property var textField: ["get", "COD"]
        property real textSize: 12
        property string textAnchor: "top"
        property var textOffset: [0, 1]
        property bool textOptional: true
    }
    
    DynamicParameter {
        type: "paint"
        property string layer: "optionalText"
        property real textHaloWidth: 2
        property string textHaloColor: "white"
    }

    
    // Gliding and UL Airfields
    
    DynamicParameter {
        type: "layer"
        
        property string name: "WPs"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["any", ["==", ["get", "CAT"], "AD-GLD"], ["==", ["get", "CAT"], "AD-INOP"], ["==", ["get", "CAT"], "AD-UL"], ["==", ["get", "CAT"], "AD-WATER"]]
    }

    DynamicParameter {
        type: "layout"

        property string layer: "WPs"
        property var textField: ["get", "NAM"]
        property real textSize: 12
        property string textAnchor: "top"
        property var textOffset: [0, 1]
        property bool textOptional: true
        property var iconImage: ["get", "CAT"]
    }
    
    DynamicParameter {
        type: "paint"
        property string layer: "WPs"
        property real textHaloWidth: 2
        property string textHaloColor: "white"
    }


    // Reporting points
    
    DynamicParameter {
        type: "layer"
        
        property string name: "RPs"
        property string layerType: "symbol"
        property string source: "aviationData"
        property int minzoom: 8
        property var filter: ["any", ["==", ["get", "CAT"], "RP"], ["==", ["get", "CAT"], "MRP"]]
    }
    
    DynamicParameter {
        type: "layout"

        property string layer: "RPs"
        property var textField: ["get", "SCO"]
        property real textSize: 12
        property string textAnchor: "top"
        property var textOffset: [0, 1]
        property bool textOptional: true
        property var iconImage: ["get", "CAT"]
    }
    
    DynamicParameter {
        type: "paint"
        property string layer: "RPs"
        property real textHaloWidth: 2
        property string textHaloColor: "white"
    }

    
    // GA Airfields with grass runway
    
    DynamicParameter {
        type: "layer"
        
        property string name: "AD-GRASS"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["any", ["==", ["get", "CAT"], "AD-GRASS"], ["==", ["get", "CAT"], "AD-MIL-GRASS"]]
    }
    
    DynamicParameter {
        type: "layout"

        property string layer: "AD-GRASS"
        property var textField: ["get", "NAM"]
        property real textSize: 12
        property string textAnchor: "top"
        property var textOffset: [0, 1]
        property bool textOptional: true
        property var iconImage: ["get", "CAT"]
        property var iconRotate: ["get", "ORI"]
        property string iconRotationAlignment: "map"
    }
    
    DynamicParameter {
        type: "paint"
        property string layer: "AD-GRASS"
        property real textHaloWidth: 2
        property string textHaloColor: "white"
    }

    
    // NavAids - Icons. Will always be drawn, but might be overdrawn by other stuff.
    
    DynamicParameter {
        type: "layer"
        
        property string name: "NavAidIcons"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["==", ["get", "TYP"], "NAV"]
    }
    
    DynamicParameter {
        type: "layout"

        property string layer: "NavAidIcons"
        property var iconImage: ["get", "CAT"]
        property bool iconIgnorePlacement: true
        property bool iconAllowOverlap: true
    }

    
    // GA Airfields with paved or unknown runway
    
    DynamicParameter {
        type: "layer"
        
        property string name: "AD-PAVED"
        property string layerType: "symbol"
        property string source: "aviationData"
        property var filter: ["any", ["==", ["get", "CAT"], "AD"], ["==", ["get", "CAT"], "AD-PAVED"], ["==", ["get", "CAT"], "AD-MIL"], ["==", ["get", "CAT"], "AD-MIL-PAVED"]]
    }
    
    DynamicParameter {
        type: "layout"

        property string layer: "AD-PAVED"
        property var textField: ["get", "NAM"]
        property real textSize: 12
        property string textAnchor: "top"
        property var textOffset: [0, 1]
        property bool textOptional: true
        property var iconImage: ["get", "CAT"]
        property var iconRotate: ["get", "ORI"]
        property string iconRotationAlignment: "map"
    }
    
    DynamicParameter {
        type: "paint"
        property string layer: "AD-PAVED"
        property real textHaloWidth: 2
        property string textHaloColor: "white"
    }
    

} // End of FlightMap
