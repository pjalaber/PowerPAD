# PowerPAD
PowerPAD is a Windows application that turns your Gamepad controller into a traditional keyboard and mouse in
order to control your desktop and programs. It supports a wide range of game controllers by relying on Xinput Windows API.

* Manage up to 4 **Xinput** compatible controllers
* Simulate mouse **move**, mouse **wheel** and key press
* Auto disable on full screen enter and enable back on leave
* On screen **virtual keyboard**
* Mouse speed and acceleration configuration
* Auto start application on Windows startup
* Online **autoupdate** application
* Integrate to Windows systray and display notifications
* Internationalization languages english and french


## Install
Download [online installer](https://pjalaber.github.io/powerpad/PowerPADSetup64.exe) for Windows 64 bits systems

## What's New in Version 0.2.0
<p><span style="font-size:large; color:white; background-color:#73AD21; padding:5px; border-radius:5px">WHAT'S NEW</span></p>
<ul style="margin-top:0px; margin-left:-10px">
<li style="margin-top:3px">Use Windows scheduled task manager in order to run PowerPAD with administrator elevated rights.<br>
Mouse can now be used in all applications running with higher rights, for instance task manager</li>
</ul>
<p><span style="font-size:large; color:white; background-color:#73AD21; padding:5px; border-radius:5px">BUG FIXES</span></p>
<ul style="margin-top:0px; margin-left:-10px">
<li style="margin-top:3px">Fix mouse pointer not being shown if no mouse device is connected</li>
<li style="margin-top:3px">Simplification of string message 'Control restored after auto-disable'</li>
<li style="margin-top:3px">Fix 'Autodisable during fullscreen' not translated at all in french</li>
<li style="margin-top:3px">Fix application window state always being displayed as 'Disconnected'</li>
<li style="margin-top:3px">Fix application single instance detection (plus race condition) not properly working</li>
</ul>

## What's New in Version 0.1.1
<p><span style="font-size:large; color:white; background-color:#73AD21; padding:5px; border-radius:5px">BUG FIXES</span></p>
<ul style="margin-top:0px; margin-left:-10px">
<li style="margin-top:3px">Update notification message when connection is restored after auto-disable on fullscreen</li>
<li style="margin-top:3px">Make fullscreen detection more reliable</li>
<li style="margin-top:3px">Fix application window being shown behind taskbar systray</li>
<li style="margin-top:3px">Fix a conflict between single button and combo buttons trigger</li>
<li style="margin-top:3px">Fix mouse move speed by using a square function giving better precision to cursor</li>
<li style="margin-top:3px">Fix virtual keyboard keys wheel going too fast</li>
<li style="margin-top:3px">Fix mouse acceleration not always working when pushing joystick in corners</li>
</ul>
