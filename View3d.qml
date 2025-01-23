import QtQuick 2.15
import QtQuick.Scene3D 2.15
import Qt3D.Render 2.15
import Qt3D.Core 2.15
import Qt3D.Input 2.15
import Qt3D.Extras 2.15

import View3dModule 1.0

Item {
    id: rootElement;
    objectName: "rootElement"

    View3dComponent {
        id: view3dComponent
        objectName: "view3dComponent"
    }

    Rectangle {
        id: scene
        objectName: "scene"

        anchors.fill: parent
        anchors.margins: 0
        color: "darkRed"

        transform: Rotation {
            id: sceneRotation
            objectName: "sceneRotation"

            axis.x: 1
            axis.y: 0
            axis.z: 0
            origin.x: scene.width
            origin.y: scene.height
        }

        Scene3D {
            id: scene3d
            objectName: "scene3d"

            anchors.fill: parent
            // anchors.margins: 10
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Entity {
                id: rootEntity
                objectName: "rootEntity"

                Camera {
                    id: camera
                    objectName: "camera"
                }

                FirstPersonCameraController {
                    id: cameraController
                    objectName: "cameraController"

                    camera: camera
                }

                components: [
                        RenderSettings {
                            activeFrameGraph: ForwardRenderer {
                                clearColor: "black"
                                camera: camera
                            }
                        },
                        // Event Source will be set by the Qt3DQuickWindow
                        InputSettings { }
                    ]
            }

            // AnimatedEntity {
            //     id: rootEntity
            // }
        }
    }

    Rectangle {
        radius: 3
        color: "#00ffffff"
        border.width: 1
        border.color: "white"
        width: childrenRect.width + anchors.margins + 30
        height: childrenRect.height + anchors.margins
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10



        Column {
            property color textColor: "white"

            x: parent.anchors.margins / 2
            y: x

            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
            Text { color: parent.textColor; text: "HELLOW: "}
        }
    }

    Component.onCompleted: {
            console.log("ApplicationWindow has been fully loaded!");
        }
}
