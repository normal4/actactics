#import <Cocoa/Cocoa.h>

@class ConsoleView;

@interface Launcher : NSObject<NSToolbarDelegate> {
    IBOutlet NSTextField *admin_password;

    IBOutlet NSWindow *window;
	
    //able to leave these disconnected
    IBOutlet NSView *view1; //Main
    IBOutlet NSView *view4; //Server
    
	
    IBOutlet NSProgressIndicator *prog; //while scanning maps - it's there if you want to wire it up
    IBOutlet NSArrayController *maps;
    IBOutlet NSPopUpButton *resolutions;
    IBOutlet NSButton *stencil;

    IBOutlet NSButton *multiplayer;
    IBOutlet ConsoleView *console;
@private	
    NSMutableDictionary *toolBarItems;
    pid_t server;
    NSMutableDictionary *fileRoles;
}

- (IBAction)playAction:(id)sender;

- (IBAction)multiplayerAction:(id)sender;

- (IBAction)playMap:(id)sender;

- (IBAction)openUserdir:(id)sender;

@end
