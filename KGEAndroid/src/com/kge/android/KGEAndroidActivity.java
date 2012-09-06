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
    public void onCreate(Bundle context) 
    {
        //System.loadLibrary(getString(R.string.app_name));        
        super.onCreate(context);

        StringBuilder id = new StringBuilder();
    	id.append(android.os.Process.myPid());//    	


/*		final AlertDialog.Builder FinishBuilder = new AlertDialog.Builder(this);
        FinishBuilder.setMessage("KGE appliction end.");
        FinishBuilder.setPositiveButton("OK", null);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(id.toString());
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() 
        {			
			public void onClick(DialogInterface arg0, int arg1) 
			{
				// TODO Auto-generated method stub
				String str = getString(R.string.kgeapp);  
				AndroidMain(str);
		        try {
					Thread.sleep(1000);
		    	} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
//				AlertDialog fin = FinishBuilder.create();
	//			fin.show();
			}
		});
        AlertDialog alert = builder.create();
        alert.show();
       
        
        //TextView  tv = new TextView(this);
        //tv.setText(id.toString());        
 */       Button btn = new Button(this);
        btn.setText(id.toString());
        btn.setOnClickListener(new OnClickListener() {
			
			//@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				String str = "memorytest";//getString(R.string.kgeapp);  
				AndroidMain(str);
				
			}
		});
        setContentView(btn);
              
   }
    
    public native void AndroidMain(String appname);

    // load libraries
    static {
		System.loadLibrary("stlport_shared");
		System.loadLibrary("kge");
    }

}