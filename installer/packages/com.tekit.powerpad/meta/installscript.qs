function Component()
{
    if (installer.isUpdater()) {
        installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false)
    }
    else if (installer.isInstaller()) {
        installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
        installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);
        installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    }
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/PowerPAD.exe",
                               "@DesktopDir@/PowerPAD.lnk", "workingDirectory=@TargetDir@");

        component.addOperation("CreateShortcut", "@TargetDir@/PowerPAD.exe",
                               "@StartMenuDir@/PowerPAD.lnk", "workingDirectory=@TargetDir@");

        component.addOperation("CreateShortcut", "@TargetDir@/PowerPAD.exe",
                               "@HomeDir@/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/PowerPAD.lnk", "workingDirectory=@TargetDir@");
    }
}

Component.prototype.installationFinishedPageIsShown = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem(component, "RunProgramCheckBoxForm", QInstaller.InstallationFinished);
        }
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            var checked = component.userInterface("RunProgramCheckBoxForm").runProgramCheckBox.checked;
            if (checked) {
                var targetDir = installer.value("TargetDir");
                installer.executeDetached(targetDir + "/PowerPAD.exe")
            }
        }
    } catch(e) {
        console.log(e);
    }
}
