#import "Managers/ReviewManager.h"


@interface ReviewQuestion : NSObject <UIAlertViewDelegate>
- (void)show;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
@end

@implementation ReviewQuestion
- (void)show {
    UIAlertView* alert = [[UIAlertView alloc]
        initWithTitle:@"Would you like to rate \n Foxy Bird?"
        message:@""
        delegate:self
        cancelButtonTitle:@"No, thanks"
        otherButtonTitles:@"Yes, sure", @"Maybe later", nil
    ];
    [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(buttonIndex == 0) {
        ReviewManager::getInstance()->onNo();
        return;
    }
    if(buttonIndex == 1) {
        [[UIApplication sharedApplication] 
            openURL:[NSURL 
                URLWithString:@"itms-apps://itunes.apple.com/app/id956980625"
            ]
        ];
        ReviewManager::getInstance()->onYes();
        return;
    }
    if(buttonIndex == 2) {
        ReviewManager::getInstance()->onLater();
        return;
    }
}

@end

void ReviewManager::showReview() {
    ReviewQuestion* rq = [[ReviewQuestion alloc] init];
    [rq show];
}