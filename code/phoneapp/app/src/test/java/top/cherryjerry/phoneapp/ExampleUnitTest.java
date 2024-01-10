package top.cherryjerry.phoneapp;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() {
        assertEquals(4, 2 + 2);
    }

    @Test
    public void MsgFrameUtilTest(){
        String str1 = "fasfgjdogbndgn{{301,0,0.5}}fsdfdssdfdsee33";
        String str2 = "hello";
        String[] args = MsgFrameUtil.splitMsgFrame(str2);
        for (String arg : args) {
            System.out.println(arg+"======");
        }
    }
}