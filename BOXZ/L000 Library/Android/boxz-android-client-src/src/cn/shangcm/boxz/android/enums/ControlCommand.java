package cn.shangcm.boxz.android.enums;

/**
 * control command
 * @version $Id$
 * @author shangcm
 */
@SuppressWarnings("javadoc")
public enum ControlCommand {

	STOP(' '),

	MOVE_FORWARD('w'),

	MOVE_FORWARD_STEP('W'),

	MOVE_BACKWARD('s'),

	MOVE_BACKWARD_STEP('S'),

	TURN_LEFT('a'),

	TURN_LEFT_STEP('A'),

	TURN_RIGHT('d'),

	TURN_RIGHT_STEP('D'),

	LEFT_ARM_UP('u'),

	LEFT_ARM_UP_STEP('U'),

	LEFT_ARM_DOWN('j'),

	LEFT_ARM_DOWN_STEP('J'),

	RIGHT_ARM_UP('i'),

	RIGHT_ARM_UP_STEP('I'),

	RIGHT_ARM_DOWN('k'),

	RIGHT_ARM_DOWN_STEP('K'),

	BOTH_ARM_UP('o'),

	BOTH_ARM_UP_STEP('O'),

	BOTH_ARM_DOWN('l'),

	BOTH_ARM_DOWN_STEP('L'),

	SPECIAL_SKILL('f');

	private char defaultValue;

	private ControlCommand(char defaultValue) {
		this.defaultValue = defaultValue;
	}

	/**
	 * This gets the defaultValue
	 * @return the defaultValue
	 */
	public char getDefaultValue() {
		return this.defaultValue;
	}

}
