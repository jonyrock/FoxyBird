/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import android.util.Log;
import android.view.View;
import android.content.Intent;
import android.os.Bundle;

import java.util.Calendar;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import com.soomla.store.billing.google.GooglePlayIabService;
import sonar.systems.framework.SonarFrameworkFunctions;
import android.app.Activity;


public class AppActivity extends Cocos2dxActivity {
  
    private static final String TAG = "Foxy Bird";
    
    private View m_decorView;
    private SonarFrameworkFunctions m_sonarFunctions;
    
    private static Activity sActivity;
    

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        m_decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        m_decorView.setSystemUiVisibility(uiOptions);
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        return glSurfaceView;
    }

    @Override
    protected void onCreate(Bundle b) {
        super.onCreate(b);
        try {
            m_sonarFunctions = new SonarFrameworkFunctions(this);
        } catch (ClassNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        m_sonarFunctions.onCreate(b);
        sActivity = this;
    }

    @Override
    protected void onPause() {
        super.onPause();
        m_sonarFunctions.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        m_sonarFunctions.onResume();
    }

    @Override
    protected void onStart() {
        super.onStart();
        m_sonarFunctions.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        m_sonarFunctions.onStop();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        m_sonarFunctions.onDestroy();
    }

    @Override
    public void onBackPressed() {
        if(m_sonarFunctions.onBackPressed()) {
            return;
        } else {
            super.onBackPressed();
        }
    }

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
        super.onActivityResult(request, response, data);
        m_sonarFunctions.onActivityResult(request, response, data);
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        m_sonarFunctions.onSaveInstanceState(outState);
    }
    
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if(android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT) {
            m_decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            );
        }
    }
    
    // CCLocalNotifications
    public static void showLocalNotification(String message, int interval, int tag) {
    		Log.v(TAG, "showLocalNotification" + message + " -> " + interval + " -> " + tag);
    		PendingIntent sender = getPendingIntent(message, tag);
    		
    		Calendar calendar = Calendar.getInstance();
    		calendar.setTimeInMillis(System.currentTimeMillis());
    		calendar.add(Calendar.SECOND, interval);
    		
    		AlarmManager am = (AlarmManager)sActivity.getSystemService(ALARM_SERVICE);
    		am.set(AlarmManager.RTC_WAKEUP, calendar.getTimeInMillis(), sender);
	  }

  	public static void cancelLocalNotification(int tag) {
    		// Log.v(TAG, "cancelLocalNotification");
        // Intent i = new Intent(sActivity.getApplicationContext(), LocalNotificationReceiver.class);
    		// i.putExtra("notification_id", tag);
    		// PendingIntent sender = PendingIntent.getBroadcast(sActivity, 0, i, 0);
    		// AlarmManager am = (AlarmManager)sActivity.getSystemService(ALARM_SERVICE);
    		// am.cancel(sender);
  	}

  	private static PendingIntent getPendingIntent(String message, int tag) {
        if(sActivity == null) {
          throw new RuntimeException("sActivity is null!");
        }
    		Intent i = new Intent(sActivity.getApplicationContext(), LocalNotificationReceiver.class);
    		i.putExtra("notification_id", tag);
    		i.putExtra("message", message);
    		PendingIntent sender = PendingIntent.getBroadcast(sActivity, 0, i, 0);
    		return sender;
  	}
    // end CCLocalNotifications
    
}