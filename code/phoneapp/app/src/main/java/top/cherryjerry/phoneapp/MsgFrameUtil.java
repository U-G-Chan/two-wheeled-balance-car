package top.cherryjerry.phoneapp;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import top.cherryjerry.phoneapp.ui.notifications.NotificationsViewModel;

/**
 * 该工具类用具处理APP与平衡车之间的通信
 * 一帧信息格式：  {{MsgCode,arg1,arg2,arg3,...}}
 */
public final class MsgFrameUtil {
    private MsgFrameUtil(){
        throw new AssertionError();
    }

    /**
     *  提取{{}}中的内容
     * @param str 字符串输入
     * @return 信息帧内容
     */
    public static String grabMsgFrame(String str){
       Pattern pattern = Pattern.compile("\\{\\{(.*?)\\}\\}");
       Matcher matcher = pattern.matcher(str);
        if (matcher.find()) {
            return matcher.group(1);
        } else {
            return "";
        }
    }

    /**
     * 处理信息帧字符串 提取MsgCode arg1 arg2 ... ,按顺序装入字符串数组
     * @param rawMsg 接收到的信息
     * @return 字符串数组
     */
    public static String[] splitMsgFrame(String rawMsg){
        String frame = grabMsgFrame(rawMsg);
        return frame.split(",");
    }

    public static void solveMsg(String rawMsg){
        String[] args =  splitMsgFrame(rawMsg);
        if(args.length > 0){
            int msgCode = Integer.parseInt(args[0]);
            switch (msgCode){
                case 401: {
                    float angleX = Float.parseFloat(args[1]);
                    float angleY = Float.parseFloat(args[2]);
                    float angleZ = Float.parseFloat(args[3]);
                    NotificationsViewModel.updateStateText(angleX,angleY,angleZ);
                    break;
                }
                case 402: {
                    float distance = Float.parseFloat(args[1]);
                    NotificationsViewModel.updateDistanceText(distance);
                    break;
                }
                case 403: {
                    break;
                }
                default:break;
            }
        }

    }

}
