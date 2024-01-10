package top.cherryjerry.phoneapp.ui.notifications;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import java.util.Locale;

import top.cherryjerry.phoneapp.BluetoothFragment;
import top.cherryjerry.phoneapp.MsgCode;

public class NotificationsViewModel extends ViewModel {


    private static MutableLiveData<String> stateText;

    private static MutableLiveData<String> distanceText;

    private static MutableLiveData<String> settingText;

    private boolean enableListenState =false;
    private boolean enableListenDistance =false;
    private boolean enableListenSetting =false;



    public NotificationsViewModel() {
        stateText = new MutableLiveData<>();
        distanceText = new MutableLiveData<>();
        settingText = new MutableLiveData<>();
        stateText.setValue("{This is stateText}");
        distanceText.setValue("{This is distanceText}");
        settingText.setValue("{This is settingText}");
    }

    public MutableLiveData<String> getStateText() {
        return stateText;
    }

    public MutableLiveData<String> getDistanceText() {
        return distanceText;
    }

    public MutableLiveData<String> getSettingText() {
        return settingText;
    }


    public static void updateStateText(float angleX, float angleY, float angleZ){
        String strAngleX = String.format(Locale.CHINA,"%.2f",angleX);
        String strAngleY = String.format(Locale.CHINA,"%.2f",angleY);
        String strAngleZ = String.format(Locale.CHINA,"%.2f",angleZ);
        stateText.setValue("angleX="+strAngleX+"\nangleY="+strAngleY+"\nangleZ="+strAngleZ);
    }

    public static void updateDistanceText(float distance){
        String strDistance = String.format(Locale.CHINA,"%.2f",distance);
        stateText.setValue("obstacle distance="+strDistance);
    }

    public void toggleEnableListenState(){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            enableListenState = !enableListenState;
            int flag = enableListenState?1:0;
            BluetoothFragment.getInstance()
                    .send("{{"+ MsgCode.SET_LISTEN_STATE.getCode()+
                            ","+flag+"}}");
        }
    }

    public void toggleEnableListenDistance(){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            enableListenDistance = !enableListenDistance;
            int flag = enableListenDistance?1:0;
            BluetoothFragment.getInstance()
                    .send("{{"+ MsgCode.SET_LISTEN_DISTANCE.getCode()+
                            ","+flag+"}}");
        }
    }

    public void toggleEnableListenSetting(){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            enableListenSetting = !enableListenSetting;
            int flag = enableListenSetting?1:0;
            BluetoothFragment.getInstance()
                    .send("{{"+ MsgCode.SET_LISTEN_SETTING.getCode()+
                            ","+flag+"}}");
        }
    }

}