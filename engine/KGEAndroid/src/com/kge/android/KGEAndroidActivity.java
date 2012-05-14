package com.kge.android;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class KGEAndroidActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
    	StringBuilder id = new StringBuilder();
    	id.append(android.os.Process.myPid());//    	
        Button btn = new Button(this);
        btn.setText(id.toString());
        btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				String str = getString(R.string.kgeapp);  
				AndroidMain(str);
				
			}
		});
        setContentView(btn);
    }

    public native void AndroidMain(String appname);

	static 
	{
		System.loadLibrary("stlport_shared");
		System.loadLibrary("kge");
	}
}
