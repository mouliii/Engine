
import os
import sys

PROJECT_NAME = "CherryEngine"
BUILD_PATH = ""
LINKED_FOLDERS = ["src", "imgui", "resources"]
SEARCH_TYPE = "RECURSIVE"


def recursive_search(path):

    files = os.listdir(path)
    file_list = []


    if os.path.basename(os.path.normpath(path)) == PROJECT_NAME:

        for file in files:
            path_to_file = os.path.join(path, file)
            if os.path.isdir(path_to_file) and file in LINKED_FOLDERS:
                file_list.extend(recursive_search(os.path.join(os.getcwd(), file)))
    else:
        for file in files:
            path_to_file = os.path.join(path, file)
            if os.path.isdir(path_to_file):
                file_list.extend(recursive_search(path_to_file))

            else:
                file_list.append(path_to_file)


    return file_list






def find_and_pop_filter(lines) -> int:
    filter_index = 0
    for i in range(len(lines)):
        if lines[i].rstrip("\n") == "#FILTERS":
            filter_index = i
            break

    lines.pop(filter_index)
    return filter_index


def create_vcxproj_files():
    files_to_add = []
    if SEARCH_TYPE == "RECURSIVE":
        files_to_add = recursive_search(os.getcwd())

    corrected_files_to_add = []

    for file in files_to_add:

        path = os.path.normpath(file)
        path_list = path.split(os.sep)

        project_path_len = len(os.getcwd().split(os.sep))

        for i in range(len(path_list)):
            if i < project_path_len:
                path_list.pop(0)

            else:
                break

        corrected_files_to_add.append(os.path.join(*path_list))


    corrected_files_to_add.sort(key=lambda f: os.path.splitext(f)[1])


    lines = []

    with open(BUILD_PATH + "vcxproj_base.txt", "r") as vcxproj:  # .vcxproj file
        lines = vcxproj.readlines()
        filter_index = find_and_pop_filter(lines)

        # begin itemgrouping .cpp files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1

        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".cpp": continue
            lines.insert(filter_index, f'\t<ClCompile Include ="{file}" />\n')
            filter_index += 1

        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1

        # begin itemgrouping .h files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1


        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".h": continue
            lines.insert(filter_index, f'\t<ClInclude Include ="{file}" />\n')
            filter_index += 1

        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1

        # begin itemgrouping .shader files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1

        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".shader": continue
            lines.insert(filter_index, f'\t<None Include ="{file}" />\n')
            filter_index += 1

        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1


       #for line in lines:
        #    print(line, end='')

    with open(os.path.join(os.getcwd(), f'{PROJECT_NAME}.vcxproj'), "w") as w_vcxproj:
        w_vcxproj.writelines(lines)

    with open(BUILD_PATH + "vcxproj-filters_base.txt", "r") as vcxproj_filters:  # .vcxproj.filters file
        lines = vcxproj_filters.readlines()
        filter_index = find_and_pop_filter(lines)

        # begin itemgrouping .cpp files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1

        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".cpp": continue
            lines.insert(filter_index, f'\t<ClCompile Include ="{file}">\n')
            filter_index += 1

            lines.insert(filter_index, f'\t  <Filter>Source Files</Filter>\n')
            filter_index += 1

            lines.insert(filter_index, f'\t</ClCompile>\n')
            filter_index += 1


        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1

        # begin itemgrouping .h files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1

        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".h": continue
            lines.insert(filter_index, f'\t<ClInclude Include ="{file}">\n')
            filter_index += 1

            lines.insert(filter_index, f'\t  <Filter>Header Files</Filter>\n')
            filter_index += 1

            lines.insert(filter_index, f'\t</ClInclude>')
            filter_index += 1

        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1

        # begin itemgrouping .shader files
        lines.insert(filter_index, "  <ItemGroup>\n")
        filter_index += 1

        for file in corrected_files_to_add:
            if os.path.splitext(file)[1] != ".shader": continue
            lines.insert(filter_index, f'\t<None Include ="{file}" />\n')
            filter_index += 1

        lines.insert(filter_index, "  </ItemGroup>\n")
        filter_index += 1

    with open(os.path.join(os.getcwd(), f'{PROJECT_NAME}.vcxproj.filters'), "w") as w_vcxproj_filters:
        w_vcxproj_filters.writelines(lines)









def main():
    global BUILD_PATH, PROJECT_PATH
    BUILD_PATH = os.path.abspath(os.getcwd()) + "\\"
    os.chdir(f'../{PROJECT_NAME}')


    create_vcxproj_files()






if __name__ == '__main__':
    main()