import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.swt.events.MouseWheelListener;

import org.eclipse.swt.widgets.Canvas;


class InputListener implements KeyListener, MouseListener, MouseMoveListener, MouseWheelListener {

	public native void sendKeyPressed(int key);
	public native void sendKeyReleased(int key);
	public native void sendMouseMoved(int x, int y);
	public native void sendMouseScrolled(int z);
	public native void sendMousePressed(int id);
	public native void sendMouseReleased(int id);
	
    /**
     * Canvas to listen to input events.
     */
    private Canvas mCanvas;
    
    InputListener()
    {
    	
    }
    
    /**
     * Returns the Canvas that is listened to.
     * 
     * @return Canvas to listen to input events.
     */
    public Canvas getCanvas() {
        return mCanvas;
    }


    /**
     * @{inheritdoc}
     * @see org.eclipse.swt.events.KeyListener#keyPressed(org.eclipse.swt.events.KeyEvent)
     */
    public void keyPressed(KeyEvent e)
    {
        sendKeyPressed(e.keyCode);
        System.out.println(e.keyCode);
    }

    /**
     * @{inheritdoc}
     * @see org.eclipse.swt.events.KeyListener#keyReleased(org.eclipse.swt.events.KeyEvent)
     */
    public void keyReleased(KeyEvent e) {
    	sendKeyReleased(e.keyCode);
    }

    /**
     * Set canvas to listen to input events.
     * 
     * @param canvas
     *            to listen to input events.
     */
    public void setCanvas(Canvas canvas) {
        if (mCanvas != canvas && mCanvas != null) {
            mCanvas.removeKeyListener(this);
            mCanvas.removeMouseListener(this);
            mCanvas.removeMouseMoveListener(this);
            mCanvas.removeMouseWheelListener(this);
        }

        mCanvas = canvas;

        if (mCanvas != null) {
        	mCanvas.addKeyListener(this);
        	mCanvas.addMouseListener(this);
        	mCanvas.addMouseMoveListener(this);
        	mCanvas.addMouseWheelListener(this);
        }
    }

    public void removeCanvas() {
    	mCanvas.removeKeyListener(this);
    	mCanvas.removeMouseListener(this);
    	mCanvas.removeMouseMoveListener(this);
    	mCanvas.removeMouseWheelListener(this);
    }
    
	/**
	 * @{inheritdoc}
	 * @see MouseListener#mouseDoubleClick(MouseEvent)
	 */
	public void mouseDoubleClick(MouseEvent event) {
		// do nothing
	}

	/**
	 * @{inheritdoc}
	 * @see MouseListener#mouseDown(MouseEvent)
	 */
	public void mouseDown(MouseEvent event) {
		sendMousePressed(event.button);
	}

	/**
	 * @{inheritdoc}
	 * @see MouseListener#mouseUp(MouseEvent)
	 */
	public void mouseUp(MouseEvent event) {
		sendMouseReleased(event.button);
	}

	/**
	 * @{inheritdoc}
	 * @see MouseMoveListener#mouseMove(MouseEvent)
	 */
	public void mouseMove(MouseEvent event) {
		sendMouseMoved(event.x, event.y);
	}
	
	public void mouseScrolled(MouseEvent e) {
		sendMouseScrolled(e.count);
	}
	
}