function Controller()
{
    installer.autoRejectMessageBoxes;
    installer.setMessageBoxAutomaticAnswer("AlreadyRunning", QMessageBox.OK);
    installer.setMessageBoxAutomaticAnswer("cancelInstallation", QMessageBox.Yes);
}

Controller.prototype.IntroductionPageCallback = function()
{
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.findChild("PackageManagerRadioButton").visible = false;
        widget.findChild("PackageManagerRadioButton").updatesEnabled = false;
        widget.findChild("UpdaterRadioButton").visible = false;
        widget.findChild("UpdaterRadioButton").updatesEnabled = false;
        if (installer.isUpdater()) {
            widget.findChild("UninstallerRadioButton").visible = false;
            widget.findChild("UninstallerRadioButton").updatesEnabled = false;
            gui.clickButton(buttons.NextButton);
        }
    }
}

Controller.prototype.ComponentSelectionPageCallback = function()
{
    var widget = gui.currentPageWidget();
    if (widget != null) {
        widget.findChild("ComponentSizeLabel").visible = false;
        widget.subTitle = " ";
        widget.title = qsTr("PowerPAD software update");
    }
}
