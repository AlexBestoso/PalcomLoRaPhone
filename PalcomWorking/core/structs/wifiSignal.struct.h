struct wifiSignal{
	String ssid = "";
	String password = "";
	signed int rssi = 0;
	int channel = 0;
	String cipher_alg = "";
};
typedef struct wifiSignal wifisignal_t;
class WiFiSignal{
	private:

	public:
		int scanForNetworks(wifisignal_t *outBuf, size_t wifiSIgnalMax){
			int ret = 0; // number of networks detected
			WiFi.mode(WIFI_STA);
                        WiFi.disconnect();
			delay(200);

			ret = WiFi.scanNetworks();
			for(int i=0; i<ret; i++){
				// Prevent Overflow
				if(i >= wifiSIgnalMax)
					break;
				
				outBuf[i].ssid = WiFi.SSID(i).c_str();
				outBuf[i].rssi = WiFi.RSSI(i);
				outBuf[i].channel = WiFi.channel(i);
				
				switch (WiFi.encryptionType(i)){
            				case WIFI_AUTH_OPEN:
                				outBuf[i].cipher_alg = "open";
                				break;
            				case WIFI_AUTH_WEP:
                				outBuf[i].cipher_alg = "WEP";
                				break;
            				case WIFI_AUTH_WPA_PSK:
                				outBuf[i].cipher_alg = "WPA";
                				break;
            				case WIFI_AUTH_WPA2_PSK:
                				outBuf[i].cipher_alg = "WPA2";
                				break;
            				case WIFI_AUTH_WPA_WPA2_PSK:
                				outBuf[i].cipher_alg = "WPA+WPA2";
                				break;
            				case WIFI_AUTH_WPA2_ENTERPRISE:
                				outBuf[i].cipher_alg = "WPA2-EAP";
                				break;
            				case WIFI_AUTH_WPA3_PSK:
                				outBuf[i].cipher_alg = "WPA3";
                				break;
            				case WIFI_AUTH_WPA2_WPA3_PSK:
                				outBuf[i].cipher_alg = "WPA2+WPA3";
                				break;
            				case WIFI_AUTH_WAPI_PSK:
                				outBuf[i].cipher_alg = "WAPI";
                				break;
            				default:
                				outBuf[i].cipher_alg = "unknown";
            			}
			}
			WiFi.scanDelete();
			return ret;
		}
};
