package com.stuffomatic.asgp.receiver;

import com.stuffomatic.asgp.ASGP;
import com.stuffomatic.event.EventManager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.util.HashMap;
import java.util.Map;

public class NotificationClicked extends BroadcastReceiver {

    @Override
    public void onReceive( Context context, Intent intent ) {

        final Map< String, String > properties = new HashMap<>();
        properties.put
            ( "notification-id", 
              String.valueOf
              ( intent.getExtras().getInt( "notification-id" ) ) );

        EventManager.tagEvent( "notification-clicked", properties );

        final Intent launchIntent = new Intent( context, ASGP.class );
        context.startActivity( launchIntent );
    }
}
