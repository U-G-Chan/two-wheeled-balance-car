package top.cherryjerry.phoneapp;

public enum MsgCode {

    SET_PID_STAND(101),
    SET_PID_VELOCITY(102),
    SET_PID_TURN(103),
    ;

    private final int msgCode;

    MsgCode(int code) {
        this.msgCode = code;
    }

    public int getCode() {
        return msgCode;
    }

}
