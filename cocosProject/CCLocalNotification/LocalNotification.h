#pragma once

#include <string>

class LocalNotification {
public:
  /**
   * Show local notification
   * @param message  Message should be shown in notificatin
   * @param interval Interval in seconds
   * @param tag      Tag to specify notification
   */
  static void show(std::string message, int interval, int tag);
  
  /**
   * Cancel local notification specified by ID
   * @param tag Tag of local notification
   */
  static void cancel(int tag);
  
  static bool askAboutNotification();
  
};

