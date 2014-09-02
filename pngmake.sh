#for OUTPUT in $(ls /mnt/Disk2 | grep Brian_Curtis)
for OUTPUT in $(ls /mnt/Disk2/Results)
do
	#if [ -d "/mnt/Disk2/$OUTPUT/Results/images" ]; then
	if [ -d "/mnt/Disk2/Results/$OUTPUT/images" ]; then
		#cd /mnt/Disk2/$OUTPUT/Results/images
		cd /mnt/Disk2/Results/$OUTPUT/images
		convert -delay 20 -loop 0 `ls rho_Diff_File*.png | sort -V` rho_Diff_Anim.gif
		echo "Created $OUTPUT/images/rho_Diff_Anim.gif"
		convert -delay 20 -loop 0 `ls Jx_Diff_File*.png | sort -V` Jx_Diff_Anim.gif
		echo "Created $OUTPUT/images/Jx_Diff_Anim.gif"
		convert -delay 20 -loop 0 `ls B_Diff_File*.png | sort -V` B_Diff_Anim.gif
		echo "Created $OUTPUT/images/B_Diff_Anim.gif"
	else
		echo "No images in $OUTPUT/"
	fi
done
