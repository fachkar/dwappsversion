#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <iterator>
#include <fstream>
namespace fs = boost::filesystem;

struct apk_appname {
    std::string installed_packge_name;
    std::string list_apk_name;
};

static apk_appname monetizedApks[] = {
    {"hungama.myplay", "hungama11june.apk"},
    {"pixatel.apps.taskmgr", "taskmgr.apk"},
    {"pixatel.apps.newsreader", "newsreader.apk"},
    {"acerapps.games.memory", "Memory.apk"},
    {"happiestminds.datawind.appstore", "AppStore.apk"},
    {"acebooks.bookreader", "BookReader.apk"},
    {"android.browser", "Browser.apk"},
    {"acerapps.bubbleblaster", "BubbleBlaster.apk"},
    {"acerapps.chessgrandmaster", "ChessGrandmaster.apk"},
    {"nuffie.android.crichq", "CricHQ_production_release.apk"},
    {"a3logics.cuttheword", "CutTheWord.apk"},
    {"nashapp.datawindeducation", "DatawindEducation_NCERT.apk"},
    {"onelearn.android.datawind.cbse", "Datawind_OneLearn_CBSE.apk"},
    {"ggb.development", "Dev_Tools.apk"},
    {"eScan.mainTab", "eScantab.apk"},
    {"android.googleanalytics", "AppAttachAnalyticsService.apk"},
    {"webaroo.replyall", "GupShup_Messenger.apk"},
    {"a3logics.hitthedifference", "HitTheDifferences.apk"},
    {"hungama.myplay_offer_datawind", "hungama.apk"},
    {"magzter.indiatodaygroup.datawind", "IndiaToday_DataWind.apk"},
    {"wps.moffice_eng", "kingsoft_office_android.apk"},
    {"mmi.maps", "MapmyIndiaDatawind.apk"},
    {"pixatel.games.minesweeper", "Minesweeper.apk"},
    {"pixatel.apps.notepad", "Notepad.apk"},
    {"kjaslabs.numberplay", "numberplay-dw-android.apk"},
    {"arjith.plus2tion", "Plus2tionDW.apk"},
    {"acerapps.solitaire", "SolitairePlus.apk"},
    {"acerapps.opensudoku", "SudokuPlus.apk"},
    {"learning2talk.talkingenglishapp", "talkingenglishapp.apk"},
    {"happiestminds.datawind.tesindia", "tesindia.apk"},
    {"TestBag", "TestBagApp.apk"},
    {"datawind.info", "UbiInfo.apk"},
    {"datawind.email", "UbiMail.apk"},
    {"druid.UbiSurfer3", "UbiSurfer.apk"},
    {"a3logics.wordgurufree", "WordGuru.apk"},
    {"pixatel.apps.Clock", "WorldClock.apk"},
    {"univexcellence", "Avdhan.apk"},
    {"android.browser", "Browser.apk"},
    {"devitsolutions.antonym", "Antonym.apk"},
    {"devitsolutions.synonym", "Synonym.apk"},
    {"devitsolutions.mathmate", "MathMate.apk"},
    {"dev.memoryhawk", "Memory_Hawk.apk"},
    {"devitSolution.rightwrong", "RightWrong.apk"},
    {"varshylmobile.smidr", "Smidr_withap_datawind.apk"},
    {"quikr", "DW_QuikrProd.apk"},
    {"ck12.ck12launcher", "ck12Launcher.apk"},
    {"goyal.myrights", "My_Rights.apk"},
    {"sonyliv", "Sony_Liv.apk"},
    {"osmand", "osmand.apk"},
    {"asbit.pvstar", "PVSTAR.apk"},
    {"skype.raider", "Skype.apk"},
    {"android.googleanalytics", "AnalyticsService.apk"},
    {"wildtangent.android", "AndroidGamesApp-appattach41.apk"},
    {"getjar.android.client", "GetJar.apk"},
    {"innovationsfrugal.lockscreen", "LockScreenApp_Signed_with_icon.apk"},
    {"slideme.sam.manager", "slidememanager.apk"},
    {"in.newsmobile", "NewsMobileDatawind.apk"},
    {"amazon.venezia", "AmazonApps-release.apk"},
    {"barbar.sarvalue", "SarValue.apk"}
};

std::string issue_command ( char* command )
{
    FILE* fpipe;
    char line[256];
    std::string result = "";

    memset ( line, 0, sizeof line );
    //printf("now executing command %s\n", command);
    if ( ! ( fpipe = ( FILE* ) popen ( command, "r" ) ) ) {
        // If fpipe is NULL
        return "error";
    }

    while ( fgets ( line, sizeof line, fpipe ) ) {
        if ( strlen ( line ) > 0 ) {
            result += line;
        }
    }
    pclose ( fpipe );
    return result;
}

int main ( int argc, char** argv )
{
    boost::progress_timer t ( std::clog );

    fs::path full_path ( fs::initial_path<fs::path>() );
    fs::path output_path ( fs::initial_path<fs::path>() );
    if ( argc > 2 ) {
        full_path = fs::system_complete ( fs::path ( argv[1] ) );
        output_path = fs::system_complete ( fs::path ( argv[2] ) );
    } else {
        std::cout << "\nusage: " << argv[0] << " [apk path] [output directory]" << std::endl;
        return 1;
    }


    std::cout << "Starting!" << std::endl;
    unsigned long file_count = 0;
    unsigned long err_count = 0;
    unsigned short numberofapps = sizeof ( monetizedApks ) / sizeof ( struct apk_appname );
    if ( !fs::exists ( full_path ) ) {
        std::cout << "\nPath Not found: " << full_path.string() << std::endl;
        return 1;
    }

    if ( fs::is_directory ( full_path ) ) {
        //none recursive, i.e., only current directory and no subdirectories
        fs::directory_iterator end_iter;
        for ( fs::directory_iterator dir_itr ( full_path ); dir_itr != end_iter; ++dir_itr ) {
            try {
                if ( fs::is_regular_file ( dir_itr->status() ) && ! ( dir_itr->path().empty() ) && strstr ( dir_itr->path().extension().c_str(), ".apk" ) ) {
                    ++file_count;
                    //std::cout << dir_itr->path().string() << "\n";
                    std::string version;
                    std::string package;
                    std::string stem_nospcefilename = dir_itr->path().stem().string();
                    boost::erase_all ( stem_nospcefilename, " " );
                    std::string tmpdir = output_path.string() + stem_nospcefilename;
                    std::string command = "/home/firas/projects/dwappversions/apktool d " + std::string ( "\"" ) + dir_itr->path().string() + std::string ( "\" " ) + tmpdir + std::string ( " 2>&1" );
                    //std::cout << tmpdir << ", " << std::endl<< "\t" << command << std::endl << std::endl;
                    std::string resulto = issue_command ( const_cast<char*> ( command.c_str() ) );
                    //std::cout << resulto << std::endl;
                    std::size_t founderror = resulto.find ( "error" );
                    if ( founderror == std::string::npos ) {
                        fs::path decoded_path ( fs::initial_path<fs::path>() );
                        decoded_path = fs::system_complete ( fs::path ( tmpdir ) );
                        if ( fs::exists ( decoded_path ) ) {
                            if ( fs::is_directory ( decoded_path ) ) {
                                fs::path AndroidManifest_path ( fs::initial_path<fs::path>() );
                                AndroidManifest_path = fs::system_complete ( fs::path ( tmpdir + "/AndroidManifest.xml" ) );
                                if ( fs::exists ( AndroidManifest_path ) ) {
                                    if ( fs::is_regular_file ( AndroidManifest_path ) ) {
                                        std::ifstream infile ( AndroidManifest_path.string().c_str() , std::ifstream::in );
                                        if ( infile ) {
                                            std::string filetext ( ( std::istreambuf_iterator<char> ( infile ) ) , std::istreambuf_iterator<char>( ) ) ;
                                            std::string tmpdelimiter = "android:versionName=\"";
                                            std::size_t foundversion = filetext.find ( tmpdelimiter , 0 );
                                            if ( foundversion != std::string::npos ) {
                                                std::size_t foundversionEnd = filetext.find ( "\"", foundversion + tmpdelimiter.length() + 1 );
                                                if ( foundversionEnd != std::string::npos ) {
                                                    version.assign ( filetext, foundversion + tmpdelimiter.length() , foundversionEnd - ( foundversion + tmpdelimiter.length() ) );

                                                    std::string tmpatdelimiter = "@string/";
                                                    std::size_t foundatversion = version.find ( tmpatdelimiter, 0 );
                                                    if ( foundatversion != std::string::npos ) {
                                                        version.assign ( version, foundatversion + tmpatdelimiter.length() , ( version.length() - tmpatdelimiter.length() ) );
                                                        fs::path strings_path ( fs::initial_path<fs::path>() );
                                                        strings_path = fs::system_complete ( fs::path ( tmpdir + "/res/values/strings.xml" ) );
                                                        if ( fs::exists ( strings_path ) ) {
                                                            if ( fs::is_regular_file ( strings_path ) ) {
                                                                std::ifstream stringsinfile ( strings_path.string().c_str() , std::ifstream::in );
                                                                if ( stringsinfile ) {
                                                                    std::string stringsfiletext ( ( std::istreambuf_iterator<char> ( stringsinfile ) ) , std::istreambuf_iterator<char>( ) ) ;
                                                                    std::string tmpVersiondelimiter = "<string name=\"" + version + "\">";
                                                                    std::size_t foundStringsVersion = stringsfiletext.find ( tmpVersiondelimiter , 0 );
                                                                    if ( foundStringsVersion != std::string::npos ) {
                                                                        std::size_t foundStringsVersionEnd = stringsfiletext.find ( "<", foundStringsVersion + tmpVersiondelimiter.length() + 1 );
                                                                        if ( foundStringsVersionEnd != std::string::npos ) {
                                                                            version.assign ( stringsfiletext, foundStringsVersion + tmpVersiondelimiter.length() , foundStringsVersionEnd - ( foundStringsVersion + tmpVersiondelimiter.length() ) );
                                                                        }
                                                                    }

                                                                }
                                                            }

                                                        }
                                                    }
                                                }

                                            }

                                            tmpdelimiter.assign ( "package=\"" );
                                            std::size_t foundpackage = filetext.find ( tmpdelimiter , 0 );
                                            if ( foundpackage != std::string::npos ) {
                                                std::size_t foundpackageEnd = filetext.find ( "\"", foundpackage + tmpdelimiter.length() + 1 );
                                                if ( foundpackageEnd != std::string::npos ) {
                                                    package.assign ( filetext, foundpackage + tmpdelimiter.length() , foundpackageEnd - ( foundpackage + tmpdelimiter.length() ) );
                                                }

                                            }
                                        }

                                    } else {
                                        std::cout << " ** ** AndroidManifest_path is not a regular file " << AndroidManifest_path.string() << std::endl;
                                    }

                                } else {
                                    std::cout << " ** ** AndroidManifest_path does not exists for " << AndroidManifest_path.string() << std::endl;
                                }
                            } else {
                                std::cout << " ** ** decoded_path is not a directory " << decoded_path.string() << std::endl;
                            }

                        } else {
                            std::cout << " ** ** decoded_path does not exists for " << decoded_path.string() << std::endl;
                        }

                    } else {
                        std::cout << " ** ** found error in command result" << std::endl;
                        std::cout << "resulto: " << resulto << std::endl;
                    }

                    std::string tmpAppName = stem_nospcefilename;
                    for ( int nmbrapps = 0; nmbrapps < numberofapps; ++nmbrapps ) {
                        std::size_t foundPkgname = package.find ( monetizedApks[nmbrapps].installed_packge_name , 0 );
                        if ( foundPkgname != std::string::npos ) {
                            tmpAppName = monetizedApks[nmbrapps].list_apk_name;
                            break;
                        }
                    }

                    std::cout << tmpAppName << "," << version << "," << package << std::endl;

                }

            } catch ( const std::exception& ex ) {
                ++err_count;
                std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
    } else { // must be a file
        std::cout << "\nPath must be directory: " << full_path.string() << std::endl;
        return 1;
    }
    std::cout << "Exiting!" << std::endl;
    return 0;
}
