#include "Console.h"

#include <sstream>
#include <codecvt>

#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <Client.hpp>

Console::Console() :
	scrollToBottom(true)
{
	commands["clear"] = std::bind(&Console::command_clear, this);
	commands["help"] = std::bind(&Console::command_help, this);
}

void Console::Update(sf::Time timeElapsed) {
	loadMessages();

	ImGui::SetNextWindowPos(ImVec2(855, 0));
	ImGui::SetNextWindowSize(ImVec2(685, 290));
	if (!ImGui::Begin("Console"))
	{
		ImGui::End();
		return;
	}

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText

	bool copy_to_clipboard = false;

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Copy")) copy_to_clipboard = true;
		if (ImGui::Selectable("Clear")) command_clear();
		ImGui::EndPopup();
	}

	if (copy_to_clipboard) ImGui::LogToClipboard();

	for (const auto &row : rows) {
		ImGui::TextUnformatted(row.c_str());
	}

	if (copy_to_clipboard) ImGui::LogFinish();

	if (scrollToBottom)
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::PushItemWidth(-1.0f);
	if (ImGui::InputText("", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
		std::string command = lowerString(buffer);
		rows.push_back(std::move(buffer));

		// call command
		auto com_iter = commands.find(command);
		if (com_iter != commands.end()) {
			com_iter->second();
		} else {
			command_notFound(command);
		}

		ImGui::SetKeyboardFocusHere(-1);
	}
	ImGui::PopItemWidth();

	ImGui::End();
}

void Console::loadMessages() {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;

	for (auto &message: CC::Get()->GetMessages()) {
		rows.push_back(myconv.to_bytes(std::move(message)));
	}

	CC::Get()->GetMessages().clear();
}

std::string Console::lowerString(const std::string &s) {
	std::string res;
	res.reserve(s.size());
	for (auto &c: s) res += tolower(c);
	return res;
}

void Console::command_help() {
	rows.push_back("----");
	rows.push_back("It's game console. You can copy the contents with right mouse button.");
	rows.push_back("Commands:");
	for (auto &pair : commands) {
		rows.push_back(std::string("- ") + pair.first);
	}
	rows.push_back("----");
}

void Console::command_clear() {
	rows.clear();
}

void Console::command_notFound(const std::string &command) {
	std::stringstream ss;
	ss << "Unknown command: " << "'" << command << "'" << std::endl;
	rows.push_back(ss.str());
}
