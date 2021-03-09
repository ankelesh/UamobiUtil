package una.unamd.qbroadcastcatcher;
import org.qtproject.qt5.android.QtNative;
import org.qtproject.qt5.android.bindings.QtActivity;
import android.os.*;
import android.content.*;
import android.app.*;
import java.lang.String;
import android.content.Intent;
import java.util.HashMap;


class BarcodeIntentInfo implements Comparable<BarcodeIntentInfo>
{
  public String intentAction;
  public String barcodeExtra;
  public String intentCategory;
  public BarcodeIntentInfo()
  {

  };
  public BarcodeIntentInfo(String intent_action, String barcode_extra, String intent_category)
  {
    barcodeExtra = barcode_extra;
    intentAction = intent_action;
    intentCategory = intent_category;
  };
  public BarcodeIntentInfo(String intent_action)
  {
      intentAction = intent_action;
  }
  @Override
  public int compareTo(BarcodeIntentInfo another)
  {
      return intentAction.compareTo(another.intentAction);
  };

  @Override
   public boolean equals(Object obj) {
       if (obj instanceof BarcodeIntentInfo)
           return intentAction.equals(((BarcodeIntentInfo) obj).intentAction);
       return super.equals(obj);
   }
   @Override
    public int hashCode()
   {
        return intentAction.hashCode();
   };
};

public class QBroadcastCatcher
{
    // native - must be implemented in Cpp via JNI
    public static native void handleTargetedExtra(String intent_name, String extra);
    public static native void handleIntentContents(String intent_name, String icontents);

    private static HashMap<String, BarcodeIntentInfo> init_intent_infos()
    {
        HashMap<String, BarcodeIntentInfo> temp = new HashMap<>();


        // defaults for newland and datalogic
       /* temp.put(
            "nlscan.action.SCANNER_RESULT",
            new BarcodeIntentInfo(
                "nlscan.action.SCANNER_RESULT",
                "SCAN_BARCODE1",
                "")
            );
        temp.put(
            "com.datalogic.decodewedge.decode_action",
            new BarcodeIntentInfo(
                "com.datalogic.decodewedge.decode_action",
                "com.datalogic.decode.intentwedge.barcode_string",
                "com.datalogic.decodewedge.decode_category")
             );
             */
        return temp;
    }

    private static HashMap<String, BarcodeIntentInfo> SCANNER_INTENTS_DESCRIPTIONS = init_intent_infos();

    private BroadcastReceiver serviceMessageReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
             if (SCANNER_INTENTS_DESCRIPTIONS.containsKey(intent.getAction()))
             {
                 if (SCANNER_INTENTS_DESCRIPTIONS.get(intent.getAction()).barcodeExtra.isEmpty())
                 {
                    StringBuilder outstr = new StringBuilder();
                    for (String key : intent.getExtras().keySet())
                    {
                       outstr.append(key).append(" _:_ ").append(intent.getExtras().get(key)).append(" __; ");
                    }
                    handleIntentContents(intent.getAction(), outstr.toString());
                 }
                 else
                 {
                    handleTargetedExtra(intent.getAction(), intent.getStringExtra(SCANNER_INTENTS_DESCRIPTIONS.get(intent.getAction()).barcodeExtra));
                 }
             }
        }
    };

    private IntentFilter createIntentFilter(String action, String category)
    {
        IntentFilter tfilter = new IntentFilter();
        tfilter.addAction(action);
        if (!category.isEmpty())
            tfilter.addCategory(category);
        return tfilter;
    };

    public void registerDefaultServiceBroadcastReceiver(Context context) {
        for (BarcodeIntentInfo binfo: SCANNER_INTENTS_DESCRIPTIONS.values())
        {
            context.registerReceiver(serviceMessageReceiver,
                                     createIntentFilter(
                                        binfo.intentAction,
                                        binfo.intentCategory
                                        )
                                     );
        }
    }
    public void registerNewServiceBroadcastReceiver(Context context, String action, String category, String extras)
    {
        SCANNER_INTENTS_DESCRIPTIONS.put(action, new BarcodeIntentInfo(action, extras, category));
        context.registerReceiver(serviceMessageReceiver,
                                 createIntentFilter(
                                    action,
                                    category
                                    )
                                 );
    }


}
