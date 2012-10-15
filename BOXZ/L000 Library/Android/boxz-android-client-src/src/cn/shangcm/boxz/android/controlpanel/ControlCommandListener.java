package cn.shangcm.boxz.android.controlpanel;

import cn.shangcm.boxz.android.enums.ControlCommand;

/**
 * control command listener
 * @version $Id$
 * @author shangcm
 */
public interface ControlCommandListener {

	/**
	 * command triggered
	 * @param command the triggered control command
	 */
	void onCommand(ControlCommand command);

}
