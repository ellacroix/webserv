std::vector<char>	vct;

// calcul size
vct.reserve(size);

ifstream ifs;

ifs.read(&*vct.begin(), size);


