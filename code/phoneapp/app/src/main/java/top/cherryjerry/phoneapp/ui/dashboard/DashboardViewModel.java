package top.cherryjerry.phoneapp.ui.dashboard;

import android.text.Editable;
import androidx.lifecycle.ViewModel;
import top.cherryjerry.phoneapp.BluetoothFragment;
import top.cherryjerry.phoneapp.MsgCode;

public class DashboardViewModel extends ViewModel {


    private static final String str_zero = "0";

    public DashboardViewModel() {
    }

    public void btConnectTest(){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            BluetoothFragment.getInstance().send("hello from APP");
        }
    }

    public void submitPIDData(MsgCode messageCode, Editable kp, Editable ki, Editable kd) {
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            String Kp = kp.toString();if(Kp.isEmpty()) Kp = str_zero;
            String Ki = ki.toString();if(Ki.isEmpty()) Ki = str_zero;
            String Kd = kd.toString();if(Kd.isEmpty()) Kd = str_zero;
            BluetoothFragment.getInstance()
                    .send("{{"+messageCode.getCode()+","+Kp+","+Ki+","+Kd+"}}");
        }
    }
}