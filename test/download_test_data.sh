mkdir -p test_data
cd test_data
smbget -u -n -R --guest smb://192.168.8.3/shares/Badania%202016/test_data
cd ..
