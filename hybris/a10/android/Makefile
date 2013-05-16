all: .downloaded

.downloaded:
	(	wget http://ssvb.name/files/20130509/system.tar.gz &&\
		tar -xzf system.tar.gz &&\
		cd system/lib &&\
		wget https://github.com/allwinner-dev-team/android_external_cedarx/raw/ef36cd760e9d76a2/CedarAndroidLib/LIB_JB_F23/libcedarv_adapter.so &&\
		wget https://github.com/allwinner-dev-team/android_external_cedarx/raw/ef36cd760e9d76a2/CedarAndroidLib/LIB_JB_F23/libcedarv_base.so &&\
		wget https://github.com/allwinner-dev-team/android_external_cedarx/raw/ef36cd760e9d76a2/CedarAndroidLib/LIB_JB_F23/libcedarxosal.so &&\
		wget https://github.com/allwinner-dev-team/android_external_cedarx/raw/ef36cd760e9d76a2/CedarAndroidLib/LIB_JB_F23/libve.so \
	)
	touch $@

clean:
	rm -rf .downloaded system system.tar.gz