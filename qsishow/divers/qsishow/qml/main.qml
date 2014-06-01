import QtQuick 1.1

Rectangle {
    id: mainWindow
    width: 1200
    height: 800
    Rectangle {
        id: header
        width: 1024
        height: 51
        color: "#ffee00"
        radius: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

		Text {
			id: headerTextEtap
            color: "#020322"
			text: "E1"
			anchors.leftMargin: 7
			anchors.fill: parent
			font.bold: false
            font.pointSize: 21
            font.family: "Arial Black"
            verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignLeft
        }
		Text {
			id: headerTextDescription
			x: 200
			color: "#020322"
			text: qsTr("HSH Vysočina cup")
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.verticalCenter: parent.verticalCenter
			font.pointSize: 21
			font.family: "Arial Black"
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignHCenter
		}
		Text {
			id: headerTextProfile
			color: "#020322"
			text: qsTr("Výsledky")
			anchors.rightMargin: 7
			anchors.fill: parent
			font.pointSize: 21
			font.family: "Arial Black"
			verticalAlignment: Text.AlignVCenter
			horizontalAlignment: Text.AlignRight
		}

    }
    Rectangle {
        id: body
        property int cellHeight: 40
        property int rowCount: height / cellHeight
        property int columnCount: 2
        property int cellWidth: width / columnCount
        color: "#1a2233"
        anchors.top: header.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        Row {
            Repeater {
                model: body.columnCount
                Column {
                    id: col
                    property int colix: index
                    Repeater {
                        model: body.rowCount
                        Cell {
                            dataIndex: index + body.rowCount * col.colix
                            height: body.cellHeight
                            width: body.cellWidth
                            Component.onCompleted: {
                                //console.log("completed: " + dataIndex)
                                timer.modelShifted.connect(loadModelData)
                            }
                        }
                    }
                }

            }
        }
    }
    Timer {
        id: timer
        signal modelShifted(variant model);
		interval: ctx_app.appConfigValue("application/refreshTime"); running: true; repeat: true
        onTriggered: {
            //ctx_app.test2 = "cau"
            var model = ctx_app.getProperty("model");
            model.shift();
            modelShifted(model);
        }
    }
	Component.onCompleted: {
		var event_info = ctx_app.eventInfo();
		var profile = ctx_app.profile();
		//console.log("profile: " + profile)
		if(profile == "results") headerTextProfile = qsTr("Výsledky");
		else if(profile == "startlist") headerTextProfile.text = qsTr("Startovní listina");
		if(event_info.EtapCount > 1) headerTextEtap.text = "E" + ctx_app.appConfigValue("event/etap");
		else headerTextEtap.text = "";
		headerTextDescription.text = event_info.EventName;
	}
}
