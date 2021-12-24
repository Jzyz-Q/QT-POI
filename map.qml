import QtQuick 2.0
import QtQuick.Window 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.0

Item {
    id: mapitem
    visible: true
    property int cnt: 0
    property int heat_max: 100
    property int row: 11
    property int col: 28
    property double lat_front
    property double lon_front
    property double lat
    property double lon

    signal rec_center(double la, double lo)
    signal rec_heatlocation(int index, int heat_cnt)
    signal start_map()
    signal start_heatmap()

    Plugin {
        id: mapPlugin
        name: "esri"// "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(lat, lon)
        zoomLevel: 3

        MapItemView {
            id: mapPoint
            model: ListModel{
                id: map_model
            }

            delegate: MapQuickItem{
                id: points
                zoomLevel: 0
                coordinate: QtPositioning.coordinate(location.latitude, location.longitude)
                anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2+15)
                sourceItem: Rectangle {
                    opacity: 0.8
                    Image {
                        id: point
                        width: 36
                        height: 48
                        source: "location1.png"
                        anchors.centerIn: parent
                    }
                }
            }
        }

        MapItemView {
            id: mapLine
            model: ListModel{
                id: map_line_model
            }

            delegate: MapPolyline {
                id: lines
                line.width: 2
                line.color: "red"
                opacity: 0.7
                path: [QtPositioning.coordinate(location_front.latitude, location_front.longitude),QtPositioning.coordinate(location_new.latitude, location_new.longitude)]
            }
        }

        MapItemView {
            id: heatmap
            model: ListModel {
                id: map_heat_model
            }

            delegate: MapQuickItem {
                id: heat
                zoomLevel: 0
                coordinate: QtPositioning.coordinate(heat_point.latitude, heat_point.longitude)
                anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
                sourceItem: Rectangle {
                    width: 60
                    height: 60
                    radius: 0.2
                    color: "red"
                    opacity: 2*ans/heat_max + 0.08
                    anchors.centerIn: parent
                }
            }
        }

    }



    onStart_map: {
        cnt = 0
        map_model.clear()
        map_line_model.clear()
    }

    onStart_heatmap: {
        cnt = 0
        map_heat_model.clear()
    }

    onRec_center: {
        //console.log(cnt)
        cnt++
        lat_front = lat
        lon_front = lon
        lat = la
        lon = lo
        map_model.append({"location":QtPositioning.coordinate(la, lo)})
        if (cnt >= 2)
            map_line_model.append({"location_new":QtPositioning.coordinate(la, lo),"location_front":QtPositioning.coordinate(lat_front, lon_front)})
    }

    onRec_heatlocation: {
        row = index/36
        col = index - 36*row
        if (heat_cnt > heat_max){
            heat_max = heat_cnt
        }
        //console.log(row)
        //console.log(col)
        if(heat_cnt>0)
            map_heat_model.append({"heat_point":QtPositioning.coordinate((-row+8)*10 +5, (-col+17)*10 +5),"ans":heat_cnt})
    }

}
