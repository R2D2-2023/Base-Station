//
// Created by jasie on 30-6-2023.
//

#include "Dropbox.h"

Dropbox::Dropbox() {
    access_token = getAccessToken();
}

void Dropbox::run(const std::string& local_file_path) {
    CURL *curl;
    CURLcode res;

    std::string dropbox_path = "/" + local_file_path;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        printf("Running curl test.\n");

        std::ifstream file(local_file_path);
        if (!file.is_open()) {
            std::cout << "Failed to open file." << std::endl;
            return;
        }

        std::string file_content((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());

        file.close();
        struct curl_slist *headers = NULL; /* init to NULL is important */
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        headers = curl_slist_append(headers, ("Dropbox-API-Arg: {\"autorename\": false, \"mode\": \"overwrite\", \"mute\": false, \"path\": \"" + dropbox_path + "\", \"strict_conflict\": false}").c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, file_content.c_str());
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Remove this line if you don't want verbose output
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);

        printf("\nFinished curl test.\n");
    }
    curl_global_cleanup();

    printf("Done!\n");
}

// Callback function to write the response into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

using json = nlohmann::json;

std::string Dropbox::getAccessToken() {

    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a CURL object
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.dropbox.com/oauth2/token");

        // Set the POST data
        std::string postFields = "refresh_token=%REFRESH_TOKEN%";
        postFields += "&grant_type=refresh_token";
        postFields += "&client_id= %APP_KEY%";
        postFields += "&client_secret=%APP_SECRET%";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());

        // Set the response callback function
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            // Request succeeded, handle the response here
//            std::cout << "Response: " << response << std::endl;

            json jsonResponse = json::parse(response);

            // Retrieve the access_token value
            std::string accessToken = jsonResponse["access_token"];
            std::cout << accessToken << std::endl;
            return accessToken;
        }
        else {
            // Request failed, handle the error here
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    // Cleanup libcurl
    curl_global_cleanup();

    return "No access token found";

}