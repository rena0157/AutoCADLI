// AutoCADLI
// By: Adam Renaud
// Created: June 2018
// Purpose: To extract data from AutoCAD's LI Function Explicitly pertaining to Unit blocks in residential
// Subdivisions


// Includes
#include <iostream> // cout
#include <fstream>  // ofstream, infstream
#include <vector>   // vector
#include <regex>
#include <Windows.h>


#ifndef NOMINMAX
#define NOMINMAX
#endif

class app
{
public:
	explicit app();

private:
	// Files
	std::string path_textfile_;
	std::string path_csvfile_;

	// Regex Paterns
	const std::regex lwpolyline_pat_{ R"((length|perimeter)\s*\d*\.\d\d\d)" };
	const std::regex line_pat_{ R"(Length\s*=\s*\d*\.\d\d\d)" };
	const std::regex hatch_pat_{ R"(Area\s*\d*\.\d\d\d)" };

	// Object vectors
	std::vector<double> lwpolylines_;
	std::vector<double> lines_;
	std::vector<double> hatches_;

	// Converts a string to a double
	double string_double(const std::string&) const;

	// Windows API opening filename
	static std::string get_filename();

	// Windows API Saves filename
	static std::string save_as();

	// Write to a CSV
	int write_csv();

	// Conversions
	double m2_ha(const double m2) { return m2 / 10000; }
};


int main()
{
	app this_app;
	return 0;
}

app::app()
{
	// Get the path to the file
	path_textfile_ = get_filename();

	// log the file to be opened and open the file
	std::cout << "Opening file: " << path_textfile_ << "\n";
	std::cout << "\n-------------------------------------\n";
	std::ifstream file(path_textfile_.c_str());

	// Read the file contents into a string vector
	std::vector<std::string> line_vector;
	for (std::string line; std::getline(file, line);)
		line_vector.push_back(line);

	// Close the file
	file.close();

	for (const auto& line : line_vector)
	{
		// lwpolylines
		if (std::regex_search(line, lwpolyline_pat_))
		{
			lines_.push_back(string_double(line));
		}

		// lines
		else if (std::regex_search(line, line_pat_))
		{
			const std::string temp = line.substr(0, line.find(','));
			lines_.push_back(string_double(temp));
		}

		// hatches
		else if (std::regex_search(line, hatch_pat_))
		{
			hatches_.push_back(string_double(line));
		}
	}

	// Total lines length
	double lines_length{ 0.0 };
	for (double num : lines_)
		lines_length += num;

	// Total hatch area
	double hatch_area{ 0.0 };
	for (double num : hatches_)
		hatch_area += num;

	// Total length
	const double total_length{ lines_length };

	//lines
	std::cout << "Extracted: " << lines_.size() << " lines\n";
	std::cout << "Total length: " << lines_length << " m\n\n";

	// Hatches
	std::cout << "Extracted: " << hatches_.size() << " hatches\n";
	std::cout << "Total Area: " << hatch_area << " ha\n\n";

	// Totals
	std::cout << "Total Objects Extracted: " << lwpolylines_.size() + lines_.size() + hatches_.size() << "\n";
	std::cout << "Total Length: " << total_length << " m\n";
	std::cout << "Total Area: " << m2_ha(hatch_area) << " ha\n\n";

	// Get csv filename and saveas
	path_csvfile_ = save_as();

	switch (write_csv())
	{
	case 0:
		std::cout << "CSV File Write Successful: " << path_csvfile_ << "\n";
		break;
	case -1:
		std::cout << "CSV File Write Unsuccessful: Need to select the same number of hatches and lines to create blocks\n";
		break;
	default: ;
	}

	system("Pause");
}

std::string app::save_as()
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof ofn);
	ZeroMemory(&filename, sizeof filename);

	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "CSV File\0*.csv\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = nullptr;
	ofn.Flags = OFN_DONTADDTORECENT;

	if (GetSaveFileName(&ofn))
		return std::string{ filename };

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

int app::write_csv()
{
	// The hatches and lines must be the same size
	if (hatches_.size() != lines_.size())
		return -1;

	// The max number of rows (not including headers)
	const auto max_rows{hatches_.size()};

	// Open the file given the file name
	std::ofstream file(path_csvfile_.c_str());

	file << "Block_Number, Frontage, Area\n";

	for (int rownum{ 0 }; rownum < max_rows; ++rownum)
		file << rownum + 1 << ", " << lines_[rownum] << ", " << m2_ha(hatches_[rownum]) << "\n";

	file.close();
	return 0;
}

// Gets the filename from the user
std::string app::get_filename()
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
double app::string_double(const std::string& str) const
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
