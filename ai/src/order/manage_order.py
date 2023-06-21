from ai.src.player import *
from ai.src.order.take_around import *
from enum import Enum

# ne pas faire lvl up les BOSS il doit toujours ce decaler
# ne prendre en compte que les IA qui ne sont pas des forksfarmer (verifier dans l'array_uuid)
# verifier si tous les aviable ai sont du meme lvl
# si oui, verifier si on a assez de ressources pour lvl up (il faut verifier par rapport au niveau la nos calculs
# sont pas bon quand c une elevation a plusieurs il faut le nombre de ressources indiqués et pas le nombre de ressources
# * le nombre de joueurs (exeption pour le niveau 1)))
# sinon elle ne sont pas du meme lvl, il faut envoyer les plus haut lvl en routine
# et pendant ce temps tu verif si les plus bas niveau ont assez de ressources pour lvl up
# si oui tu les lvl up et qu'ils sont assez
# si ils sont pas assez tu leurs demande de fork
# si ils sont assez mais pas assez de ressources tu les envoies en routine

def get_available_ia(player):
    from ai.src.player import EnumOrder
    available_ai = []
    for player_array in player.array_uuid:
        if str(player_array["job"]) == EnumOrder.NOTHING.value and player_array["pos"] == 0:
            available_ai.append(player_array)
    return available_ai

def get_ressources(player):
    from ai.src.player import EnumObject
    _, foot_case = look_item(player)
    print("FOOT CASE IN GET RESSOURCES: {}".format(foot_case))
    boss_case = []
    boss_case.append(foot_case.count("player"))
    for i in EnumObject:
        boss_case.append(foot_case.count(i.value))
    print("BOSS CASE IN GET RESSOURCES: {}".format(boss_case))
    return boss_case

def check_minus_level(available_ia):
    minus_level = 10

    for player in available_ia:
        if player["level"] < minus_level:
            minus_level = player["level"]
    return minus_level

def check_same_level(available_ia, minus_level):
    array_bigger_level = []
    array_minus_level = []
    for player in available_ia:
        if player["level"] != minus_level:
            array_bigger_level.append(player)
        else:
            array_minus_level.append(player)
    return array_bigger_level, array_minus_level

def send_them_in_routine(boss, array_player):
    from ai.src.player import EnumOrder, EnumHeader
    from ai.src.game import msg_create
    count = 0
    order = EnumHeader.ORDER.value
    # call look at me here
    for player in array_player:
        if count == 0:
            # boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.SQUARE_COLLECT.value, "1"))
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.TAKE_FAR.value, "1"))
        elif count == 1:
            # TAKE_FAR ROUTINE
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.SQUARE_COLLECT.value, "3"))
        elif count == 2:
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.SQUARE_COLLECT.value, "7"))
        elif count == 3:
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.TAKE_AROUND.value))
        elif count == 4:
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.SQUARE_COLLECT.value, "5"))
        else:
            boss.broadcast(msg_create(boss, player["uuid"], order, EnumOrder.TAKE_FAR.value))
        count += 1

def handle_result_level_up(player, result):
    from ai.src.game import msg_create
    from ai.src.player import EnumOrder, EnumHeader
    if result == -1:
        msg_create(player, player["uuid"], EnumHeader.ORDER.value, EnumOrder.FORK.value)
    elif result == 0:
        msg_create(player, player["uuid"], EnumHeader.ORDER.value, EnumOrder.SQUARE_COLLECT.value)
    else:
        msg_create(player, player["uuid"], EnumHeader.ORDER.value, EnumOrder.LEVEL_UP.value)

class Answer(Enum):
    INCANTATION = 1
    ROUTINE = 2
    FORK = 3
    NOTHING = 4

def handle_level_up(actual_lvl, boss_case, nbr_player):
    from ai.src.player import levelUpArray

    if (nbr_player == 0):
        return Answer.NOTHING.value, 0
    if (actual_lvl == 1):
        if (boss_case[2] < levelUpArray[0][2] * nbr_player):
            print("ROUTINE")
            print("boss case: ", boss_case)
            return Answer.ROUTINE.value, 0
        else:
            return Answer.INCANTATION.value, 0
    if (nbr_player < levelUpArray[actual_lvl - 1][0]):
        return Answer.FORK.value, nbr_player % levelUpArray[actual_lvl - 1][0]
    for i in range(len(levelUpArray[0])):
        if (boss_case[i] < levelUpArray[actual_lvl - 1][i] * (nbr_player // levelUpArray[actual_lvl - 1][0])):
            return Answer.ROUTINE.value, 0
    return Answer.INCANTATION.value, 0

def call_levelup(boss, level, array_minus_level):
    from ai.src.player import EnumOrder, EnumHeader
    from ai.src.game import msg_create
    for i in range (len(array_minus_level) // level):
                boss.broadcast(msg_create(boss, array_minus_level[i]["uuid"], EnumHeader.ORDER.value, EnumOrder.LEVEL_UP.value))

def ia_all_same_level(boss, array_minus_level, result, nbr_fork):
    from ai.src.game import msg_create
    from ai.src.player import EnumOrder, EnumHeader

    if (result == Answer.INCANTATION.value):
        if (array_minus_level[0]["level"] == 1):
            for player in array_minus_level:
                boss.broadcast(msg_create(boss, player["uuid"], EnumHeader.ORDER.value, EnumOrder.LEVEL_UP.value))
        elif array_minus_level[0]["level"] == 2 or array_minus_level[0]["level"] == 3:
            call_levelup(boss, 2, array_minus_level)
        elif array_minus_level[0]["level"] == 4 or array_minus_level[0]["level"] == 5:
            call_levelup(boss, 4, array_minus_level)
        elif array_minus_level[0]["level"] == 6 or array_minus_level[0]["level"] == 7:
            call_levelup(boss, 6, array_minus_level)
    elif (result == Answer.FORK.value):
        count = 0
        for player in range(len(array_minus_level)):
            if (count < nbr_fork):
                boss.broadcast(msg_create(boss, array_minus_level[player]["uuid"], EnumHeader.ORDER.value, EnumOrder.FORK.value))
                count += 1
            else:
                boss.broadcast(msg_create(boss, array_minus_level[player]["uuid"], EnumHeader.ORDER.value, EnumOrder.SQUARE_COLLECT.value, "1"))
    else:
        send_them_in_routine(boss, array_minus_level)

def ai_not_same_level(boss, boss_case, array_minus_level, array_bigger_level):
    from ai.src.game import msg_create
    from ai.src.player import EnumOrder, EnumHeader

    result, nbr_fork = handle_level_up(int(array_minus_level[0]["level"]), boss_case, len(array_minus_level))
    cpt_ai_lvl_7 = 0
    for player in array_bigger_level:
        if player["level"] == 7:
            cpt_ai_lvl_7 += 1
    if (cpt_ai_lvl_7 >= 1):
        boss_case = get_ressources(boss)
        result, nbr_fork = handle_level_up(7, boss_case, 7)

        if (result == Answer.INCANTATION.value):
            boss.broadcast(msg_create(boss, array_bigger_level[0]["uuid"], EnumHeader.ORDER.value, EnumOrder.LEVEL_UP.value))
        else:
            send_them_in_routine(boss, array_bigger_level)
            send_them_in_routine(boss, array_minus_level)
            return 1
    send_them_in_routine(boss, array_bigger_level)
    if (result == Answer.INCANTATION.value):
        if (array_minus_level[0]["level"] == 1):
            for player in array_minus_level:
                boss.broadcast(msg_create(boss, player["uuid"], EnumHeader.ORDER.value, EnumOrder.LEVEL_UP.value))
        elif array_minus_level[0]["level"] == 2 or array_minus_level[0]["level"] == 3:
            call_levelup(boss, 2, array_minus_level)
        elif array_minus_level[0]["level"] == 4 or array_minus_level[0]["level"] == 5:
            call_levelup(boss, 4, array_minus_level)
        elif array_minus_level[0]["level"] == 6 or array_minus_level[0]["level"] == 7:
            call_levelup(boss, 6, array_minus_level)
    elif (result == Answer.FORK.value):
        count = 0
        for player in range(len(array_minus_level)):
            if (count < nbr_fork):
                boss.broadcast(msg_create(boss, array_minus_level[player]["uuid"], EnumHeader.ORDER.value, EnumOrder.FORK.value))
                count += 1
            else:
                boss.broadcast(msg_create(boss, array_minus_level[player]["uuid"], EnumHeader.ORDER.value, EnumOrder.SQUARE_COLLECT.value, "1"))
    else:
        send_them_in_routine(boss, array_minus_level)

def manage_order(boss):
    from ai.src.player import EnumObject

    available_ai = get_available_ia(boss)
    if (len(available_ai) != len(boss.array_uuid) and boss.level7 != 7):
        print("Not all ai are available")
        print("All ai : ", boss.array_uuid)
        return 0
    boss_case = get_ressources(boss)
    minus_level = check_minus_level(available_ai)
    array_bigger_level, array_minus_level = check_same_level(available_ai, minus_level)
    if (len(array_minus_level) == 0):
        return 0
    result, nbr_fork = handle_level_up(minus_level, boss_case, len(array_minus_level))

    if (len(array_bigger_level) == 0):
        ia_all_same_level(boss, array_minus_level, result, nbr_fork)
    else:
        ai_not_same_level(boss, boss_case, array_minus_level, array_bigger_level)
        pass

    if boss_case[1] != 0:
        for i in range (boss_case[1]):
            boss.take(EnumObject.FOOD.value)
    if ((array_minus_level[0]["level"]) == 1):
        take_around(boss, 4)
    print("\n")
    print("\n")
    return 1
