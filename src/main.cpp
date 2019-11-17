#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>

#include <tgbot/tgbot.h>


int randint(int max) {
    srand(time(0));
    return rand() % max;
}

std::string randChoice(std::string player_name, int player_state) {
    std::string deaths[4] = {player_name + "'s guts were spilled all over the floor!",
                             player_name + " just got killed...",
                             player_name + " just got served!",
                             player_name + " bites the dust!"};
    std::string escapes[3] = {player_name + " will live to see another day!",
                              player_name + " escaped death!",
                              player_name + " just got lucky!"};
    if (player_state == 0) {
        return deaths[randint(4)];
    } else {
        return escapes[randint(4)];
    }
}

int main() {
    std::string token = "1234566:AVGVSDUVSYAVAYDSVYAdsaVUSHhwvaysdvy";
    TgBot::Bot bot(token);

    bot.getEvents().onInlineQuery([&bot](const TgBot::InlineQuery::Ptr &query) {


        std::vector<TgBot::InlineQueryResult::Ptr> results;

        auto text = std::make_shared<TgBot::InputTextMessageContent>();
        auto result = std::make_shared<TgBot::InlineQueryResultArticle>();


        if (randint(5 + 1 == 6)) {
            text->messageText = randChoice(query->from->firstName, 0);
        } else {
            text->messageText = randChoice(query->from->firstName, 1);
        }

        result->title = "Russian Roulette!";
        result->id = "1";
        result->inputMessageContent = text;
        results.push_back(result);
        bot.getApi().answerInlineQuery(query->id, results, 1, true);

    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (std::exception &e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}