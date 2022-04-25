
<?php
$target_dir = "./uploads_folder/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
	$check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
	if($check !== false) {
		echo "File is an image - " . $check["mime"] . ".";
		$uploadOk = 1;
	} else {
		echo "File is not an image.";
		$uploadOk = 0;
	}
}

// Check if file already exists
if (file_exists($target_file)) {
	echo "Sorry, file already exists.";
	$uploadOk = 0;
}

// Check file size
if ($_FILES["fileToUpload"]["size"] > 500000) {
	echo "Sorry, your file is too large.";
	$uploadOk = 0;
}

// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
	&& $imageFileType != "gif" ) {
	echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.";
	$uploadOk = 0;
}



// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
	echo "Sorry, your file was not uploaded.";
	// if everything is ok, try to upload file
} else {
	//if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file))
	if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], "./uploads_folder/lala.jpg"))
	{
		echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.";
	}
	else
	{
		echo "Sorry, there was an error uploading your file.\n";
		echo "<br>";
		echo "target_file = " . $target_file . "\n";
		echo "<br>";
		$cwd = getcwd();
		echo "cwd = " . $cwd;
		echo "<br>";
		echo "tmp_name = " . $_FILES["fileToUpload"]["tmp_name"];
		echo "<br>";
		echo "name = " . $_FILES["fileToUpload"]["name"];
		echo "<br>";
		echo "error = " . $_FILES["fileToUpload"]["error"];
		echo "<br>";
		echo "realpath(\$target_file) = " . realpath($target_file);
		echo "<br>";
		if (is_uploaded_file($_FILES["fileToUpload"]["tmp_name"]))
		{
			echo "file was temporarily uploaded";
			echo "<br>";
		}
		else
		{
			echo "file was not temporarily uploaded";
			echo "<br>";
		}
		phpinfo(INFO_VARIABLES);
	}
}
?>
