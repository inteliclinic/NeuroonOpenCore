#ifndef CONFIG_H
#define CONFIG_H

//TODO: add a json parser and get the values from a configuration file
class Config {
public:

	static Config& instance() {
		static Config m_instance;
		return m_instance;
	}


	int neuroon_eeg_freq() {
		return 125;
	}

	int neuroon_ir_freq() {
		return 25;
	}

private:
	Config() {}
};


#endif
