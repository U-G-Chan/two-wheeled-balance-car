package top.cherryjerry.phoneapp.ui.home;
import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import java.util.Set;

import top.cherryjerry.phoneapp.R;
import top.cherryjerry.phoneapp.databinding.FragmentHomeBinding;

public class HomeFragment extends Fragment {

    private FragmentHomeBinding binding;
    private ActivityResultLauncher<String> requestPermissionLauncher;
    private BluetoothAdapter bluetoothAdapter;

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

        // 初始化权限请求的回调
        requestPermissionLauncher = registerForActivityResult(new ActivityResultContracts.RequestPermission(), isGranted -> {
            if (isGranted) {
                // 权限被授予，可以执行需要权限的操作
                scanBluetoothDevices();
            } else {
                // 权限被拒绝，向用户解释为何需要此权限
                Toast.makeText(getContext(), "Location permission is needed for Bluetooth scanning", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void onAttach(@NonNull Context context) {
        super.onAttach(context);
        BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
        if (bluetoothManager != null) {
            bluetoothAdapter = bluetoothManager.getAdapter();
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
        HomeViewModel homeViewModel =
                new ViewModelProvider(this).get(HomeViewModel.class);

        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        final TextView output_terminal = root.findViewById(R.id.output_terminal);
        homeViewModel.getText().observe(getViewLifecycleOwner(),output_terminal::setText);

        ScrollView scrollView = root.findViewById(R.id.scrollView);
        homeViewModel.setScrollView(scrollView);

        //监听模式按键
        final Button button_mode1 = root.findViewById(R.id.button_mode1);
        final Button button_mode2 = root.findViewById(R.id.button_mode2);
        final Button button_mode3 = root.findViewById(R.id.button_mode3);
        button_mode1.setOnClickListener(view -> homeViewModel.useMode1());
        button_mode2.setOnClickListener(view -> homeViewModel.useMode2());
        button_mode3.setOnClickListener(view -> homeViewModel.useMode3());
        //监听方向按键
        final Button button_up = root.findViewById(R.id.button_up);
        final Button button_down = root.findViewById(R.id.button_down);
        final Button button_left = root.findViewById(R.id.button_left);
        final Button button_right = root.findViewById(R.id.button_right);
        button_up.setOnClickListener(view -> homeViewModel.keyUP());
        button_down.setOnClickListener(view -> homeViewModel.keyDown());
        button_left.setOnClickListener(view -> homeViewModel.keyLeft());
        button_right.setOnClickListener(view -> homeViewModel.keyRight());
        //监听信息输出控制键
        final Button button_clear = root.findViewById(R.id.button_clear);
        button_clear.setOnClickListener(view -> homeViewModel.cleanMessage());
        @SuppressLint("UseSwitchCompatOrMaterialCode") final Switch switch_message_flag
                = root.findViewById(R.id.switch_message_flag);
        switch_message_flag.setOnCheckedChangeListener((view, checked) ->
                homeViewModel.toggleEnableMessageFlag());


/*
        homeViewModel.getStepLiveData().observe(getViewLifecycleOwner(),
                newStep -> output_terminal.setText(String.valueOf(newStep)));
*/

        return root;
    }


/*    Button scanButton = root.findViewById(R.id.button_mode1);
        scanButton.setOnClickListener(v -> {
        if (bluetoothAdapter != null && bluetoothAdapter.isEnabled()) {
            scanBluetoothDevices();
        } else {
            Toast.makeText(getContext(), "Please enable Bluetooth", Toast.LENGTH_SHORT).show();
        }
    });*/

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

    // 在Fragment中请求权限
    private void checkPermissionsAndScan() {
        if (ContextCompat.checkSelfPermission(requireContext(),
                Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
            // 已经获得权限，可以扫描
            scanBluetoothDevices();
        } else {
            // 直接请求权限
            requestPermissionLauncher.launch(Manifest.permission.ACCESS_FINE_LOCATION);
        }
    }

    private void scanBluetoothDevices() {
        // 检查权限
        if (ContextCompat.checkSelfPermission(requireContext(),
                Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // 权限未被授予
            Log.d("BluetoothDevice", "Permission not granted for accessing location.");
            return;
        }
        // 权限已被授予，继续执行操作
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                Log.d("BluetoothDevice", "Device: " + device.getName() + ", " + device.getAddress());
            }
        } else {
            Log.d("BluetoothDevice", "No paired Bluetooth devices found.");
        }
    }

}