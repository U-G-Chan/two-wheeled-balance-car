package top.cherryjerry.phoneapp.ui.home;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TextView;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import top.cherryjerry.phoneapp.MsgCode;
import top.cherryjerry.phoneapp.R;
import top.cherryjerry.phoneapp.databinding.FragmentHomeBinding;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;
    private HomeViewModel homeViewModel;

    private final ActivityResultLauncher<Intent> enableBtResultLauncher = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if (result.getResultCode() == Activity.RESULT_OK) {
                    System.out.println("蓝牙已成功启用");
                } else {
                    System.out.println("用户没有启用蓝牙或者操作被取消");
                }
            }
    );

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onAttach(@NonNull Context context) {
        super.onAttach(context);
        BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
        if (bluetoothManager != null) {
            BluetoothAdapter bluetoothAdapter = bluetoothManager.getAdapter();
            if (bluetoothAdapter != null) {
                System.out.println("get bluetoothAdapter");
            }else{
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                enableBtResultLauncher.launch(enableBtIntent); // 使用 ActivityResultLauncher
            }
        }
    }


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        if(homeViewModel == null){
            homeViewModel = new ViewModelProvider(this).get(HomeViewModel.class);
        }

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        ScrollView scrollView = root.findViewById(R.id.scrollView);
        homeViewModel.setScrollView(scrollView);

        final TextView output_terminal = root.findViewById(R.id.output_terminal);
        homeViewModel.getText().observe(getViewLifecycleOwner(),output_terminal::setText);


        //监听模式按键
        final Button button_mode1 = root.findViewById(R.id.button_mode1);
        final Button button_mode2 = root.findViewById(R.id.button_mode2);
        final Button button_mode3 = root.findViewById(R.id.button_mode3);
        button_mode1.setOnClickListener(view -> homeViewModel.useMode(MsgCode.SET_MODE_CTRL));
        button_mode2.setOnClickListener(view -> homeViewModel.useMode(MsgCode.SET_MODE_FREE));
        button_mode3.setOnClickListener(view -> homeViewModel.useMode(MsgCode.SET_MODE_LINE));
        //监听方向按键
        final Button button_up = root.findViewById(R.id.button_up);
        final Button button_down = root.findViewById(R.id.button_down);
        final Button button_left = root.findViewById(R.id.button_left);
        final Button button_right = root.findViewById(R.id.button_right);
        button_up.setOnClickListener(view -> homeViewModel.addSpeed(MsgCode.KEY_UP));
        button_down.setOnClickListener(view -> homeViewModel.addSpeed(MsgCode.KEY_DOWN));
        button_left.setOnClickListener(view -> homeViewModel.addSpeed(MsgCode.KEY_LEFT));
        button_right.setOnClickListener(view -> homeViewModel.addSpeed(MsgCode.KEY_RIGHT));
        //监听信息输出控制键
        final Button button_clear = root.findViewById(R.id.button_clear);
        button_clear.setOnClickListener(view -> homeViewModel.cleanMessage());
        @SuppressLint("UseSwitchCompatOrMaterialCode") final Switch switch_message_flag
                = root.findViewById(R.id.switch_message_flag);
        switch_message_flag.setOnCheckedChangeListener((view, checked) ->
                homeViewModel.toggleEnableMessageFlag());
        return root;
    }
    
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

}