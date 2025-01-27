import QtQuick 2.15
import QtQuick.Scene3D 2.15
import QtQuick.Controls 2.15
import Qt3D.Render 2.15
import Qt3D.Core 2.15
import Qt3D.Input 2.15
import Qt3D.Extras 2.15


Item {
    id: rootElement;
    objectName: "rootElement"

    // Timer {
    //         id: updateTimer
    //         interval: 100 // Интервал в миллисекундах (100 мс = 0.1 секунды)
    //         running: true // Таймер запускается автоматически
    //         repeat: true // Таймер будет повторяться
    //         onTriggered: {
    //             // Обновляем текст с текущим значением m_cameraPositionX
    //             // cameraPositionText.text = "camera pos x: " + view3dComponent.m_cameraPositionX;
    //             // console.log("camera pos x: " + view3dComponent.m_cameraPositionX);
    //         }
    //     }

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
            spacing: 5

            Text {
                color: parent.textColor;
                text: "Camera { " +
                      View3Dcpp.m_cameraPosition.x.toFixed(1) + "; " +
                      View3Dcpp.m_cameraPosition.y.toFixed(1) + "; " +
                      View3Dcpp.m_cameraPosition.z.toFixed(1) + " }";
            }
            Text {
                color: parent.textColor;
                text: View3Dcpp.m_selectedEntityText;
            }
        }
    }

    // Нижний правый угол (порт)
    Row {
        id: row
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Text {
            id: textItem
            color: "white"
            text: "port: "

            wrapMode: Text.NoWrap
            // y: y + 33
            // x: x + 20
            y: 33
            x: 20
        }

        TextField {
            id: port
            color: "white"
            text: View3Dcpp.m_portText
            height: textItem.height
            width: 50
            leftPadding: 3
            topPadding: 0
            bottomPadding: 0
            verticalAlignment: TextInput.AlignVCenter
            enabled: false

            y: 33
            x: 25
            font.pixelSize: 15
            topInset: 1

            maximumLength: 5  // Ограничение на 5 символов

            background: Rectangle {
                color: "transparent"  // Установите цвет фона на прозрачный
            }

            onAccepted: {
                port.enabled = false
                port.focus = false
                scene3d.focus = true

                View3Dcpp.updatePortSettings(port.text);
            }

            validator: IntValidator {
                bottom: 0  // Минимальное значение
                top: 99999  // Максимальное значение (5 цифр)
            }
        }

        Button {
            id: button
            // icon.source: "file:///home/user061/Загрузки/free-icon-edit-button-7124470.png"  // Путь к иконке
            icon.source: "qrc:/ico/ico/editIco.png"
            icon.width: width  // Ширина иконки равна высоте текста
            icon.height: height  // Высота иконки равна высоте текста
            implicitWidth: height  // Ширина кнопки равна высоте кнопки (квадратная кнопка)
            implicitHeight: 50 // Ширина кнопки равна высоте кнопки (квадратная кнопка)
            y: 16
            x: 10

            background: Rectangle {
                color: "transparent"  // Установите цвет фона на прозрачный
            }
            icon.color: "white"  // Делаем иконку белой

            // Добавляем ToolTip
            ToolTip {
                id: toolTip
                text: "Change port"  // Текст подсказки
                delay: 0  // Задержка перед появлением подсказки (в миллисекундах)
                timeout: 0  // Время, через которое подсказка исчезнет (в миллисекундах)
                visible: false  // По умолчанию подсказка скрыта

                // Уменьшаем размер ToolTip
                implicitWidth: 80  // Ширина ToolTip
                implicitHeight: 20  // Высота ToolTip

                // Настраиваем шрифт
                contentItem: Text {
                    text: toolTip.text
                    font.pixelSize: 10  // Уменьшаем размер шрифта
                    color: "white"  // Цвет текста
                    horizontalAlignment: Text.AlignHCenter  // Выравнивание текста по центру
                    verticalAlignment: Text.AlignVCenter  // Выравнивание текста по центру
                }

                // Настраиваем фон ToolTip
                background: Rectangle {
                    color: "#70000000"  // Цвет фона ToolTip
                    radius: 2  // Закругление углов
                }
            }

            // Показываем подсказку при наведении
            MouseArea {
                width: 15  // Ширина области (3 пикселя влево и 3 пикселя вправо от центра)
                height: 15  // Высота области (3 пикселя вверх и 3 пикселя вниз от центра)
                anchors.centerIn: parent  // Центрируем MouseArea относительно кнопки
                hoverEnabled: true
                onEntered: {
                    // Устанавливаем позицию ToolTip под курсором
                    toolTip.x = mouseX - toolTip.width / 2  // Центрируем по горизонтали
                    toolTip.y = parent.y - 20  // Смещаем немного ниже курсора
                    toolTip.visible = true
                }
                onExited: {
                    toolTip.visible = false
                }
                onClicked: {
                    port.enabled = true
                    port.focus = true
                }

                // onPositionChanged: {
                //     // Обновляем позицию ToolTip, если курсор двигается
                //     toolTip.x = mouseX - toolTip.width / 2
                //     toolTip.y = mouseY - 50
                // }
            }
        }
    }

    Component.onCompleted: {
        console.log("ApplicationWindow has been fully loaded!");

        // Для нормального позиционирования
        textItem.x = 30
        port.x = 65
        button.x = 100
    }
}
