package cn.shangcm.boxz.android.controlpanel;

/**
 * control panel
 * @version $Id$
 * @author shangcm
 */
public interface ControlPanel {

	/**
	 * to set the Control Command Listener
	 * @param commandListener the Control Command Listener
	 */
	void setCommandListener(ControlCommandListener commandListener);

	/**
	 * to start the control panel
	 */
	void start();

	/**
	 * to stop the control panel
	 */
	void stop();

}
