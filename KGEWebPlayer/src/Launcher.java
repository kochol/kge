import org.eclipse.swt.*;
import org.eclipse.swt.widgets.*;

import java.lang.Exception;
import java.applet.*;
import java.io.*;


public class Launcher extends Applet {
	
	protected static final long serialVersionUID = 5121819178012096909L;
	
	//Screen size of the embedded window;
	protected int mWidth = 800;
	protected int mHeight = 600;
	
	//Here you should specify your resource zip files;
	protected String[] mResources = {"SdkTrays.zip", "Sinbad.zip"};          /*getParameter("media").split(",");*/
	
	protected static final int READ_BUFFER_SIZE = 1000;

	protected boolean mIsDisposing = false;
	public Display mDisplay;
	protected Canvas mCanvas; 
	
	//It's the full resource path
	protected String mResourcePath;
	
	protected java.awt.Canvas mAWTParent;
	protected org.eclipse.swt.widgets.Shell mSWTParent;
	
	//The following are native methods defined in the KGEWebApp.dll
	public native void WebMain();
	//The following are native methods defined in the KGEWebPlayer.dll
	public native void initialise();
	public native boolean update();
	public native void shutdown();
	public native void setCanvas(int hwnd, int width, int height);
	public native void setResourcePath(String fullpath);	
	
	public Launcher()
	{

	}
	
	public void init() 
	{	
		Thread thread = new Thread(new Runnable() 
	    {
	    	public void run() 
	    	{
	    		try
	    	    {
	    			//Extract media files;
	    			extractResources();
	    			
	    			//Load DLLs manually;
	    			loadSystemLibraries();
	    			
	    			//Setup AWT and SWT, and send SWT parameters to Ogre via setCanvas (native method);
			        setupGUI();
			  
			        //Input Listener for SWT
			        InputListener inpListener = new InputListener();
			        inpListener.setCanvas(mCanvas);

			        // Native methods;
			        initialise();
			        renderLoop();
			        shutdown();	

		//	        inpListener.removeCanvas();
	    	    }
	    	    catch (Exception ex)
	    	    {
	    	    	
	    	    }
	    	}
	    });
		thread.start();
	}
	
	public void stop() 
	{
	    if (mDisplay != null && !mDisplay.isDisposed()) 
	    {
	    	mDisplay.syncExec(new Runnable() 
	    	{
	    		public void run() 
	    		{
	    			if (mSWTParent != null && !mSWTParent.isDisposed())
	    					mSWTParent.dispose();
	    			mSWTParent = null;
	    			mDisplay.dispose();
	    			mDisplay = null;
	    		}
	    	});
	    	remove(mAWTParent);
	    	mAWTParent = null;
	    }
	}
    
	protected void extractResources()
    {
    	try
    	{
	        // find a unique temporary file name 
    		File file = null;
	    	InputStream in = null;
	    	OutputStream out = null;
	    	
	    	for (int i = 0; i < mResources.length; i++)
	    	{
		    	in = this.getClass().getResourceAsStream(mResources[i]);
		        if (in != null)
		        { 
		          file = new File(System.getProperty("java.io.tmpdir") + mResources[i]);
		          
		          // write the output file
		          out = new FileOutputStream(file.getPath());
		          int c;
		          byte[] buffer = new byte[READ_BUFFER_SIZE];
		          while ((c = in.read(buffer)) != -1)
		        	  out.write(buffer, 0, c);
		          in.close();
		          out.close();
		        }
		        mResourcePath = file.getPath();
		        mResourcePath = mResourcePath.substring(0, mResourcePath.length()- mResources[i].length());
	    	}
    	}
    	catch (Exception ex)
    	{
    		
    	}
    }
    
	protected void loadSystemLibraries()
	{
		//Load DLL/so
		System.loadLibrary("KGEWebPlayer");
		
		//Native method;
		//setResourcePath(mResourcePath);
		WebMain();
	}
	
	protected void setupGUI()
	{
		setLayout(new java.awt.GridLayout(1, 1));
		mAWTParent = new java.awt.Canvas();
		add(mAWTParent);
		mDisplay = new org.eclipse.swt.widgets.Display();
        mSWTParent = org.eclipse.swt.awt.SWT_AWT.new_Shell(mDisplay, mAWTParent);
        mSWTParent.setLayout(new org.eclipse.swt.layout.FillLayout());
        mCanvas = new Canvas(mSWTParent, SWT.NO_BACKGROUND);
        mSWTParent.open();			     
        
        setCanvas(mCanvas.handle, mWidth, mHeight);
        
        setSize(mWidth, mHeight);

        validate();
	}
    
	protected void renderLoop()
	{
        while (mAWTParent.isShowing()) 
        {
        	if(!update())
        		break;
            mCanvas.redraw();
	        mDisplay.readAndDispatch();
        }
	}
}
