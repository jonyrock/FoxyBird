#include "LocalNotification.h"

void LocalNotification::show(std::string message, int interval, int tag) {
    UILocalNotification* notification = [[UILocalNotification alloc] init];

    notification.fireDate = [[NSDate date] dateByAddingTimeInterval:interval];
    notification.timeZone = [NSTimeZone defaultTimeZone];
    notification.alertBody = [NSString 
        stringWithCString:message.c_str()
        encoding:[NSString defaultCStringEncoding]
    ];
    notification.alertAction = @"Open";
    notification.soundName = UILocalNotificationDefaultSoundName;

    NSNumber* tag1 = [NSNumber numberWithInteger:tag];
    NSDictionary* infoDict = [NSDictionary dictionaryWithObject:tag1 forKey:@"ID"];
    notification.userInfo = infoDict;

    [[UIApplication sharedApplication] scheduleLocalNotification:notification];

    [notification release];
}

void LocalNotification::cancel(int tag) {
    for(UILocalNotification* notification in [[UIApplication sharedApplication] scheduledLocalNotifications]) {
        if([[notification.userInfo objectForKey:@"ID"] integerValue] == tag) {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

bool LocalNotification::askAboutNotification() {
    UIUserNotificationType requestType =
          UIUserNotificationTypeAlert | 
          UIUserNotificationTypeBadge | 
          UIUserNotificationTypeSound;

    if ([UIApplication instancesRespondToSelector:@selector(registerUserNotificationSettings:)]) {
        UIApplication* application = [UIApplication sharedApplication];
        
        [application registerUserNotificationSettings:
            [UIUserNotificationSettings 
                settingsForTypes: requestType 
                categories:nil
            ]
        ];
    }
    UIRemoteNotificationType types = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
    return types == requestType;
}
