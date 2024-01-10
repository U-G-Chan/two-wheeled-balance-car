package top.cherryjerry.phoneapp;

public enum MsgCode {

    SET_PID_STAND(101),
    SET_PID_VELOCITY(102),
    SET_PID_TURN(103),

    SET_MODE_CTRL(201),
    SET_MODE_FREE(202),
    SET_MODE_LINE(203),
    SET_LISTEN_STATE(204),
    SET_LISTEN_DISTANCE(205),
    SET_LISTEN_SETTING(206),

    KEY_RLS(300),
    KEY_UP(301),
    KEY_DOWN(302),
    KEY_LEFT(303),
    KEY_RIGHT(304),

    UPDATE_STATE(401),
    UPDATE_DISTANCE(402),
    UPDATE_SETTING(403),
    ;

    private final int msgCode;

    MsgCode(int code) {
        this.msgCode = code;
    }

    public int getCode() {
        return msgCode;
    }

}
