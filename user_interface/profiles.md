## Profiles

The Frostbite application has support for client profiles. Each client profile is a set of settings that
contain application properties for macros, text highlights, text alterations, quick buttons and window colors.
It's important to note that not all settings take effect retrospectively when profile is changed. This mostly
applies to settings that affect the way game text is handled in real time such as text alterations and
highlights. Once text is substituted or ignored it's not possible to revert it by changing a profile.

### Creating a new profile

* navigate to "Profiles" menu from the main menu bar
* press "Create new profile"
* enter profile name

> In order to create a new profile the application needs to have write access to hard disk if write permissions
> are not granted by the operating system, a user profile can also be created manually by adding a new folder into
> the Frostbite application profiles folder.

A user profile can be loaded from the "Profiles" -> "Load profile" menu. Only one profile can be active at a time however it
is not required to have an active profile selected for the application to work. In that case the application uses the
default settings.

To avoid any complications or issues during the deletion process the Frostbite application does not support
removal of user profiles directly. Profile removal can be done manually by deleting the appropriate profile folder
from the Frostbite application profiles folder. It is recommended to close the application prior to removal of a
user profile or deactivate any active profiles from the "Profile" -> "Load profile" menu.
