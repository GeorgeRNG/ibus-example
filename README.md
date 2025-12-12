
this code is based on  
https://github.com/neutrinolabs/xrdp/blob/devel/sesman/chansrv/input_ibus.c  
https://github.com/ibus/ibus-tmpl  


————————————————————————————————————————————————————————————————————


alas, I shall type this out with the miniture example project I made, safe from double spaces, semicolons, and with a combo for a large amount of em dashes.

all the available/installed IMEs are in a folder at `/usr/share/ibus/component` (which is the entire install process in install.sh)  
the installed IMEs are provided with XML files, which can provide multiple IMEs or just different options of a single IME. 

the actual IME is just a process running on the system. ibus will start with with the <exec> seen in XML file mentioned earlier. It connects via dbus, but that isn't important because it's handled by the library so we can just use some functions to get it to work.

once you have worked out to install the file, and to call the correct arcane functions, you can now register events and do all sorts of fun stuff with the user's typing.  
it's simple but rather obscurely documented.


————————————————————————————————————————————————————————————————————


the main.c file already contains a few features for reference. If you want to entirely remove what I've done to get a pure IME that does nothing, remove these things:
- the is_key_held_down variable at the top
- all the system() calls
- all code inside the key_event_cb. replace it with return FALSE (as TRUE would prevent you typing whatsoever)