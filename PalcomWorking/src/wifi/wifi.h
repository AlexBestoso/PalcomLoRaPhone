class PalcomWifi{
	private:
		String ssid;
		String password;

		String targetHost;
		int targetPort;
	public:
		PalcomWifi(void);
		~PalcomWifi();	
		void init(void);
		void denit(void);
};
