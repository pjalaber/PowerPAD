function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/PowerPAD.exe", "@DesktopDir@/PowerPAD.lnk", "workingDirectory=@TargetDir@");
    }
}
