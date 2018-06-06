#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <Windows.h>


#ifndef NOMINMAX
#define NOMINMAX
#endif

/**
* \brief Opens the get filename dialogbox and allows the user to select a file from the filesystem
* \return a string to the file selected
*/
std::string get_filename();

/*
* returns a double from a string
*/
double string_double(const std::string&);


int main()
{
	// Get the path to the file
	std::string path = get_filename();

	// log the file to be opened and open the file
	std::cout << "Opening file: " << path << "\n";
	std::cout << "\n-------------------------------------\n";
	std::ifstream file(path.c_str());

	// Read the file contents into a string vector
	std::vector<std::string> line_vector;
	for (std::string line; std::getline(file, line);)
		line_vector.push_back(line);

	// regex patterns
	const std::regex lwpolyline_pat{ R"((length|perimeter)\s*\d*\.\d\d\d)" };
	const std::regex line_pat{R"(Length\s*=\s*\d*\.\d\d\d)"};
	const std::regex hatch_pat{R"(Area\s*\d*\.\d\d\d)"};


	// Search for the patterns and extract into vectors
	std::vector<double> lwpolylines;
	std::vector<double> lines;
	std::vector<double> hatches;

	for (const auto& line : line_vector)
	{
		// lwpolylines
		if (std::regex_search(line, lwpolyline_pat))
		{
			lwpolylines.push_back(string_double(line));
		}

		// lines
		else if (std::regex_search(line, line_pat))
		{
			const std::string temp = line.substr(0, line.find(','));
			lines.push_back(string_double(temp));
		}

		// hatches
		else if (std::regex_search(line, hatch_pat))
		{
			hatches.push_back(string_double(line));
		}
	}

	// Total the polylines length
	double lwpolylines_length{0.0};
	for (double num : lwpolylines)
		lwpolylines_length += num;

	// Total lines length
	double lines_length{ 0.0 };
	for (double num : lines)
		lines_length += num;

	// Total hatch area
	double hatch_area{ 0.0 };
	for (double num : hatches)
		hatch_area += num;

	// Total length
	const double total_length{ lwpolylines_length + lines_length };

	// lwpolylines
	std::cout << "Extracted: " << lwpolylines.size() << " lwpolylines\n";
	std::cout << "Total length: " << lwpolylines_length << " m\n\n";

	//lines
	std::cout << "Extracted: " << lines.size() << " lines\n";
	std::cout << "Total length: " << lines_length << " m\n\n";

	// Hatches
	std::cout << "Extracted: " << hatches.size() << " hatches\n";
	std::cout << "Total Area: " << hatch_area << " ha\n\n";

	// Totals
	std::cout << "Total Objects Extracted: " << lwpolylines.size() + lines.size() + hatches.size() << "\n";
	std::cout << "Total Length: " << total_length << " m\n";
	std::cout << "Total Area: " << hatch_area << " ha\n\n";

	system("Pause");
}

// Gets the filename from the user
std::string get_filename()
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof ofn);
	ZeroMemory(&filename, sizeof filename);

	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "Text Files\0*.txt\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
		return std::string{ filename };
	// All this stuff below is to tell you exactly how you messed up above. 
	// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
	switch (CommDlgExtendedError())
	{
	case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";
		break;
	case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";
		break;
	case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";
		break;
	case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";
		break;
	case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";
		break;
	case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";
		break;
	case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n";
		break;
	case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";
		break;
	case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";
		break;
	case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";
		break;
	case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";
		break;
	case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";
		break;
	case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";
		break;
	case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n";
		break;
	case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n";
		break;
	default: std::cout << "You cancelled.\n";
	}
	return std::string{ "" };
}

// Converts a string to a double
double string_double(const std::string& str)
{
	std::string return_string;
	for (const auto ch : str)
	{
		if (isdigit(ch))
			return_string.push_back(ch);
		else if (ch == '.')
			return_string.push_back(ch);
	}
	return std::stod(return_string);
}
