package top.cherryjerry.phoneapp.ui.dashboard;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import top.cherryjerry.phoneapp.BluetoothFragment;

public class DashboardViewModel extends ViewModel {

    private final MutableLiveData<String> mText;

    public DashboardViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is dashboard fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }

    public void btConnectTest(){
        if (BluetoothFragment.connected == BluetoothFragment.Connected.True) {
            BluetoothFragment.getInstance().send("hello from APP");
        }
    }
}