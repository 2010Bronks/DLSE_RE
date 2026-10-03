#pragma once

enum EPacketType
{
	D6sys_void,
	D6sys_id,
	D6sys_ploc,
	D6sys_addpc,
	D6sys_ackno,
	D6sys_ppc,
	D6sys_pall,
	D6sys_delpc,
	D6sys_dropitem,
	D6sys_rqp,
	D6sys_iswap,
	D6sys_waitcon,
	D6sys_pwait,
	D6sys_nowait,
	D6sys_newcon,
	D6sys_reqcon,
	D6sys_mdpack,
	D6sys_pcpack,
	D6sys_dstack,
	D6sys_npcmsg,
	D6sys_rlock,
	D6sys_reply,
	D6sys_npctalk,
	D6sys_addkey,
	D6sys_npcbye,
	D6sys_npcadios,
	D6sys_steal,
	D6sys_stealack,
	D6sys_defend,
	D6sys_defendack,
	D6sys_srqinit,
	D6sys_srqdone,
	D6sys_gterpack,
	D6sys_gmonpack,
	D6sys_combmsg,
	D6sys_pentry,
	D6sys_pexit,
	D6sys_pman,
	D6sys_tradinit,
	D6sys_traditem,
	D6sys_traddone,
	D6sys_divinit,
	D6sys_divdeli,
	D6sys_giveitpc,
	D6sys_divend,
	D6sys_divpdone,
	D6sys_divchoose,
	D6sys_pcitem,
	D6sys_pcinv,
	D6sys_reqitemack,
	D6sys_useitem,
	D6sys_useack,
	D6sys_spell,
	D6sys_spellack,
	D6sys_reqitem,
	D6sys_delterro,
	D6sys_fight,
	D6sys_fightack,
	D6sys_addparty,
	D6sys_delparty,
	D6sys_newitem,
	D6sys_detachitem,
	D6sys_monclear,
	D6sys_newmons,
	D6sys_awardexp,
	D6sys_npcinv,
	D6sys_mslattach,
	D6sys_mslorigin,
	D6sys_mslmove,
	D6sys_newattach,
	D6sys_objanim,
	D6sys_monxlate,
	D6sys_movent,
	D6sys_hitsw,
	D6sys_hitswack,
	D6sys_xpolys,
	D6sys_gentpack,
	D6sys_textmsg,
	D6sys_switchpack,
	D6sys_sfx,
	D6sys_efx,
	D6sys_inspect,
	D6sys_inspectack,
	D6sys_mount,
	D6sys_mountack,
	D6sys_domount,
	D6sys_pcaop,
	D6sys_pushpack,
	D6sys_srqparty,
	D6sys_segload,
	D6sys_talent,
	D6sys_zeroinvitem,
	D6sys_review,
	D6sys_pcupdate,
	D6sys_hpmax,
	D6sys_addoext,
	D6sys_deloext,
	D6sys_goxtpack,
	D6sys_pctalk,
	D6sys_latchprop,
	D6sys_latchpropack,
	D6sys_pclatch,
	D6sys_objtwid,
	D6sys_monfade,
	D6sys_pcorders,
	D6sys_giveitem,
	D6sys_giveack,
	D6sys_pcgold,
	D6sys_mountpack,
	D6sys_takeitpc,
	D6sys_invcharges,
	D6sys_eqitem,
	D6sys_orderops,
	D6sys_polymorph,
	D6sys_gameinfo,
	D6sys_npcmdr,
	D6sys_npckillmsgq,
	D6sys_npcregs,
	D6sys_chatmsg,
	D6sys_polymons,
	D6sys_i2item,
	D6sys_swapweap,
	D6sys_damitem,
	D6sys_startgame,
	D6sys_pkills,
	D6sys_player_ready,
	D6sys_all_players_r,
	D6sys_monster_diff,
	D6sys_enc_freq,
	D6sys_leave_pregame,
	D6sys_pregame_msg,
	D6sys_reqallgameopt,
	D6sys_pcgamereg,
	D6sys_pcqflag,
	D6sys_torchlight,
	D6sys_affli,
	D6sys_vbite,
	D6sys_hide,
	D6sys_specop,
	D6sys_disarmtrap,
	D6sys_disarmtrapack,
	D6sys_pcstat,
	D6sys_monvanrem,
	D6sys_reqswapweap,
	D6sys_requiver,
	D6sys_equipquiver,
	D6sys_npctrade,
	D6sys_npcbuysell,
	D6sys_npcbuyack,
	D6sys_smash,
	D6sys_smashack,
	D6sys_newprop,
	D6sys_fountain,
	D6sys_fountainack,
	D6sys_polycet,
	D6sys_chestspell,
	D6sys_chestspellack,
	D6sys_monswapweap,
	D6sys_chestuse,
	D6sys_chestuseack,
	D6sys_spiriteye,
	D6sys_newmissile,
	D6sys_block,
	D6sys_blockack,
	D6sys_firegun,
	D6sys_firegunack,
	D6sys_npcinittalk,
	D6sys_npcdonetalk,
	D6sys_npctalkmode,
	D6sys_setpcmode,
	D6sys_npctrain,
	D6sys_npcbuytrain,
	D6sys_npcbuytrainac,
	D6sys_npcdonetrade,
	D6sys_loadnpcinv,
	D6sys_netchat,
	D6sys_animpack,
	D6sys_endpartanim,
	D6sys_endpartloopan,
	D6sys_actionanim,
	D6sys_baseanim,
	D6sys_propanim,
	D6sys_boneemitstart,
	D6sys_boneemitend,
	D6sys_mononfire,
	D6sys_blockend,
	D6sys_ladder,
	D6sys_ladderack,
	D6sys_swtrailstart,
	D6sys_swtrailend,
	D6sys_firesword,
	D6sys_npclexiop,
	D6sys_questlog,
	xxxxx_maptag,
	xxxxx_terrmapblock,
	xxxxx_terrmapbdone,
	D6sys_tmbmonpack,
	xxxxx_tmbobjpack,
	D6sys_srqterrmap,
	D6sys_rideobj,
	D6sys_monequip,
	D6sys_hitmsg,
	D6sys_falldam,
	D6sys_blastpc,
	D6sys_objstream,
	D6sys_npcflags,
	D6sys_gamestatus,
	D6sys_heraldry,
	xxxxx_tmbparty,
	D6sys_moonbr,
	D6sys_moonbrack,
	D6sys_mbteleport,
	D6sys_moonbrlock,
	D6sys_deprune,
	D6sys_mononice,
	D6sys_camp,
	D6sys_camplock,
	D6sys_pcclass,
	D6sys_mongobjflags,
	D6sys_revivepc,
	D6sys_reviveack,
	D6sys_saveinfo,
	D6sys_reqpc,
	xxxxx_mploadpos,
	D6sys_mploadposack,
	D6sys_pcfull,
	D6sys_pcfullack,
	D6sys_npckillspeech,
	D6sys_npccountflag,
	D6sys_relicboon,
	D6sys_tmbend,
	D6sys_tmblock,
	D6sys_tmback,
	D6sys_multiops,
	D6sys_itemup,
	D6sys_statmsg,
	D6sys_powerattack,
	D6sys_reqloot,
	D6sys_reqlootack,
	D6sys_lootstatus,
	D6sys_pcmoney,
	D6sys_target,
	D6sys_pcattr,
	D6sys_monremake,
	D6sys_vulnerable,
	D6sys_moventpack,
	D6sys_maptag,
	D6sys_terrmapblock,
	D6sys_terrmapbdone,
	D6sys_tmbobjpack,
	D6sys_tmbparty,
	D6sys_mploadpos,
};

static const std::string PacketNumToStr(EPacketType num)
{
#define PACKET_NUM_CASE(name) case EPacketType::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(D6sys_void);
		PACKET_NUM_CASE(D6sys_id);
		PACKET_NUM_CASE(D6sys_ploc);
		PACKET_NUM_CASE(D6sys_addpc);
		PACKET_NUM_CASE(D6sys_ackno);
		PACKET_NUM_CASE(D6sys_ppc);
		PACKET_NUM_CASE(D6sys_pall);
		PACKET_NUM_CASE(D6sys_delpc);
		PACKET_NUM_CASE(D6sys_dropitem);
		PACKET_NUM_CASE(D6sys_rqp);
		PACKET_NUM_CASE(D6sys_iswap);
		PACKET_NUM_CASE(D6sys_waitcon);
		PACKET_NUM_CASE(D6sys_pwait);
		PACKET_NUM_CASE(D6sys_nowait);
		PACKET_NUM_CASE(D6sys_newcon);
		PACKET_NUM_CASE(D6sys_reqcon);
		PACKET_NUM_CASE(D6sys_mdpack);
		PACKET_NUM_CASE(D6sys_pcpack);
		PACKET_NUM_CASE(D6sys_dstack);
		PACKET_NUM_CASE(D6sys_npcmsg);
		PACKET_NUM_CASE(D6sys_rlock);
		PACKET_NUM_CASE(D6sys_reply);
		PACKET_NUM_CASE(D6sys_npctalk);
		PACKET_NUM_CASE(D6sys_addkey);
		PACKET_NUM_CASE(D6sys_npcbye);
		PACKET_NUM_CASE(D6sys_npcadios);
		PACKET_NUM_CASE(D6sys_steal);
		PACKET_NUM_CASE(D6sys_stealack);
		PACKET_NUM_CASE(D6sys_defend);
		PACKET_NUM_CASE(D6sys_defendack);
		PACKET_NUM_CASE(D6sys_srqinit);
		PACKET_NUM_CASE(D6sys_srqdone);
		PACKET_NUM_CASE(D6sys_gterpack);
		PACKET_NUM_CASE(D6sys_gmonpack);
		PACKET_NUM_CASE(D6sys_combmsg);
		PACKET_NUM_CASE(D6sys_pentry);
		PACKET_NUM_CASE(D6sys_pexit);
		PACKET_NUM_CASE(D6sys_pman);
		PACKET_NUM_CASE(D6sys_tradinit);
		PACKET_NUM_CASE(D6sys_traditem);
		PACKET_NUM_CASE(D6sys_traddone);
		PACKET_NUM_CASE(D6sys_divinit);
		PACKET_NUM_CASE(D6sys_divdeli);
		PACKET_NUM_CASE(D6sys_giveitpc);
		PACKET_NUM_CASE(D6sys_divend);
		PACKET_NUM_CASE(D6sys_divpdone);
		PACKET_NUM_CASE(D6sys_divchoose);
		PACKET_NUM_CASE(D6sys_pcitem);
		PACKET_NUM_CASE(D6sys_pcinv);
		PACKET_NUM_CASE(D6sys_reqitemack);
		PACKET_NUM_CASE(D6sys_useitem);
		PACKET_NUM_CASE(D6sys_useack);
		PACKET_NUM_CASE(D6sys_spell);
		PACKET_NUM_CASE(D6sys_spellack);
		PACKET_NUM_CASE(D6sys_reqitem);
		PACKET_NUM_CASE(D6sys_delterro);
		PACKET_NUM_CASE(D6sys_fight);
		PACKET_NUM_CASE(D6sys_fightack);
		PACKET_NUM_CASE(D6sys_addparty);
		PACKET_NUM_CASE(D6sys_delparty);
		PACKET_NUM_CASE(D6sys_newitem);
		PACKET_NUM_CASE(D6sys_detachitem);
		PACKET_NUM_CASE(D6sys_monclear);
		PACKET_NUM_CASE(D6sys_newmons);
		PACKET_NUM_CASE(D6sys_awardexp);
		PACKET_NUM_CASE(D6sys_npcinv);
		PACKET_NUM_CASE(D6sys_mslattach);
		PACKET_NUM_CASE(D6sys_mslorigin);
		PACKET_NUM_CASE(D6sys_mslmove);
		PACKET_NUM_CASE(D6sys_newattach);
		PACKET_NUM_CASE(D6sys_objanim);
		PACKET_NUM_CASE(D6sys_monxlate);
		PACKET_NUM_CASE(D6sys_movent);
		PACKET_NUM_CASE(D6sys_hitsw);
		PACKET_NUM_CASE(D6sys_hitswack);
		PACKET_NUM_CASE(D6sys_xpolys);
		PACKET_NUM_CASE(D6sys_gentpack);
		PACKET_NUM_CASE(D6sys_textmsg);
		PACKET_NUM_CASE(D6sys_switchpack);
		PACKET_NUM_CASE(D6sys_sfx);
		PACKET_NUM_CASE(D6sys_efx);
		PACKET_NUM_CASE(D6sys_inspect);
		PACKET_NUM_CASE(D6sys_inspectack);
		PACKET_NUM_CASE(D6sys_mount);
		PACKET_NUM_CASE(D6sys_mountack);
		PACKET_NUM_CASE(D6sys_domount);
		PACKET_NUM_CASE(D6sys_pcaop);
		PACKET_NUM_CASE(D6sys_pushpack);
		PACKET_NUM_CASE(D6sys_srqparty);
		PACKET_NUM_CASE(D6sys_segload);
		PACKET_NUM_CASE(D6sys_talent);
		PACKET_NUM_CASE(D6sys_zeroinvitem);
		PACKET_NUM_CASE(D6sys_review);
		PACKET_NUM_CASE(D6sys_pcupdate);
		PACKET_NUM_CASE(D6sys_hpmax);
		PACKET_NUM_CASE(D6sys_addoext);
		PACKET_NUM_CASE(D6sys_deloext);
		PACKET_NUM_CASE(D6sys_goxtpack);
		PACKET_NUM_CASE(D6sys_pctalk);
		PACKET_NUM_CASE(D6sys_latchprop);
		PACKET_NUM_CASE(D6sys_latchpropack);
		PACKET_NUM_CASE(D6sys_pclatch);
		PACKET_NUM_CASE(D6sys_objtwid);
		PACKET_NUM_CASE(D6sys_monfade);
		PACKET_NUM_CASE(D6sys_pcorders);
		PACKET_NUM_CASE(D6sys_giveitem);
		PACKET_NUM_CASE(D6sys_giveack);
		PACKET_NUM_CASE(D6sys_pcgold);
		PACKET_NUM_CASE(D6sys_mountpack);
		PACKET_NUM_CASE(D6sys_takeitpc);
		PACKET_NUM_CASE(D6sys_invcharges);
		PACKET_NUM_CASE(D6sys_eqitem);
		PACKET_NUM_CASE(D6sys_orderops);
		PACKET_NUM_CASE(D6sys_polymorph);
		PACKET_NUM_CASE(D6sys_gameinfo);
		PACKET_NUM_CASE(D6sys_npcmdr);
		PACKET_NUM_CASE(D6sys_npckillmsgq);
		PACKET_NUM_CASE(D6sys_npcregs);
		PACKET_NUM_CASE(D6sys_chatmsg);
		PACKET_NUM_CASE(D6sys_polymons);
		PACKET_NUM_CASE(D6sys_i2item);
		PACKET_NUM_CASE(D6sys_swapweap);
		PACKET_NUM_CASE(D6sys_damitem);
		PACKET_NUM_CASE(D6sys_startgame);
		PACKET_NUM_CASE(D6sys_pkills);
		PACKET_NUM_CASE(D6sys_player_ready);
		PACKET_NUM_CASE(D6sys_all_players_r);
		PACKET_NUM_CASE(D6sys_monster_diff);
		PACKET_NUM_CASE(D6sys_enc_freq);
		PACKET_NUM_CASE(D6sys_leave_pregame);
		PACKET_NUM_CASE(D6sys_pregame_msg);
		PACKET_NUM_CASE(D6sys_reqallgameopt);
		PACKET_NUM_CASE(D6sys_pcgamereg);
		PACKET_NUM_CASE(D6sys_pcqflag);
		PACKET_NUM_CASE(D6sys_torchlight);
		PACKET_NUM_CASE(D6sys_affli);
		PACKET_NUM_CASE(D6sys_vbite);
		PACKET_NUM_CASE(D6sys_hide);
		PACKET_NUM_CASE(D6sys_specop);
		PACKET_NUM_CASE(D6sys_disarmtrap);
		PACKET_NUM_CASE(D6sys_disarmtrapack);
		PACKET_NUM_CASE(D6sys_pcstat);
		PACKET_NUM_CASE(D6sys_monvanrem);
		PACKET_NUM_CASE(D6sys_reqswapweap);
		PACKET_NUM_CASE(D6sys_requiver);
		PACKET_NUM_CASE(D6sys_equipquiver);
		PACKET_NUM_CASE(D6sys_npctrade);
		PACKET_NUM_CASE(D6sys_npcbuysell);
		PACKET_NUM_CASE(D6sys_npcbuyack);
		PACKET_NUM_CASE(D6sys_smash);
		PACKET_NUM_CASE(D6sys_smashack);
		PACKET_NUM_CASE(D6sys_newprop);
		PACKET_NUM_CASE(D6sys_fountain);
		PACKET_NUM_CASE(D6sys_fountainack);
		PACKET_NUM_CASE(D6sys_polycet);
		PACKET_NUM_CASE(D6sys_chestspell);
		PACKET_NUM_CASE(D6sys_chestspellack);
		PACKET_NUM_CASE(D6sys_monswapweap);
		PACKET_NUM_CASE(D6sys_chestuse);
		PACKET_NUM_CASE(D6sys_chestuseack);
		PACKET_NUM_CASE(D6sys_spiriteye);
		PACKET_NUM_CASE(D6sys_newmissile);
		PACKET_NUM_CASE(D6sys_block);
		PACKET_NUM_CASE(D6sys_blockack);
		PACKET_NUM_CASE(D6sys_firegun);
		PACKET_NUM_CASE(D6sys_firegunack);
		PACKET_NUM_CASE(D6sys_npcinittalk);
		PACKET_NUM_CASE(D6sys_npcdonetalk);
		PACKET_NUM_CASE(D6sys_npctalkmode);
		PACKET_NUM_CASE(D6sys_setpcmode);
		PACKET_NUM_CASE(D6sys_npctrain);
		PACKET_NUM_CASE(D6sys_npcbuytrain);
		PACKET_NUM_CASE(D6sys_npcbuytrainac);
		PACKET_NUM_CASE(D6sys_npcdonetrade);
		PACKET_NUM_CASE(D6sys_loadnpcinv);
		PACKET_NUM_CASE(D6sys_netchat);
		PACKET_NUM_CASE(D6sys_animpack);
		PACKET_NUM_CASE(D6sys_endpartanim);
		PACKET_NUM_CASE(D6sys_endpartloopan);
		PACKET_NUM_CASE(D6sys_actionanim);
		PACKET_NUM_CASE(D6sys_baseanim);
		PACKET_NUM_CASE(D6sys_propanim);
		PACKET_NUM_CASE(D6sys_boneemitstart);
		PACKET_NUM_CASE(D6sys_boneemitend);
		PACKET_NUM_CASE(D6sys_mononfire);
		PACKET_NUM_CASE(D6sys_blockend);
		PACKET_NUM_CASE(D6sys_ladder);
		PACKET_NUM_CASE(D6sys_ladderack);
		PACKET_NUM_CASE(D6sys_swtrailstart);
		PACKET_NUM_CASE(D6sys_swtrailend);
		PACKET_NUM_CASE(D6sys_firesword);
		PACKET_NUM_CASE(D6sys_npclexiop);
		PACKET_NUM_CASE(D6sys_questlog);
		PACKET_NUM_CASE(xxxxx_maptag);
		PACKET_NUM_CASE(xxxxx_terrmapblock);
		PACKET_NUM_CASE(xxxxx_terrmapbdone);
		PACKET_NUM_CASE(D6sys_tmbmonpack);
		PACKET_NUM_CASE(xxxxx_tmbobjpack);
		PACKET_NUM_CASE(D6sys_srqterrmap);
		PACKET_NUM_CASE(D6sys_rideobj);
		PACKET_NUM_CASE(D6sys_monequip);
		PACKET_NUM_CASE(D6sys_hitmsg);
		PACKET_NUM_CASE(D6sys_falldam);
		PACKET_NUM_CASE(D6sys_blastpc);
		PACKET_NUM_CASE(D6sys_objstream);
		PACKET_NUM_CASE(D6sys_npcflags);
		PACKET_NUM_CASE(D6sys_gamestatus);
		PACKET_NUM_CASE(D6sys_heraldry);
		PACKET_NUM_CASE(xxxxx_tmbparty);
		PACKET_NUM_CASE(D6sys_moonbr);
		PACKET_NUM_CASE(D6sys_moonbrack);
		PACKET_NUM_CASE(D6sys_mbteleport);
		PACKET_NUM_CASE(D6sys_moonbrlock);
		PACKET_NUM_CASE(D6sys_deprune);
		PACKET_NUM_CASE(D6sys_mononice);
		PACKET_NUM_CASE(D6sys_camp);
		PACKET_NUM_CASE(D6sys_camplock);
		PACKET_NUM_CASE(D6sys_pcclass);
		PACKET_NUM_CASE(D6sys_mongobjflags);
		PACKET_NUM_CASE(D6sys_revivepc);
		PACKET_NUM_CASE(D6sys_reviveack);
		PACKET_NUM_CASE(D6sys_saveinfo);
		PACKET_NUM_CASE(D6sys_reqpc);
		PACKET_NUM_CASE(xxxxx_mploadpos);
		PACKET_NUM_CASE(D6sys_mploadposack);
		PACKET_NUM_CASE(D6sys_pcfull);
		PACKET_NUM_CASE(D6sys_pcfullack);
		PACKET_NUM_CASE(D6sys_npckillspeech);
		PACKET_NUM_CASE(D6sys_npccountflag);
		PACKET_NUM_CASE(D6sys_relicboon);
		PACKET_NUM_CASE(D6sys_tmbend);
		PACKET_NUM_CASE(D6sys_tmblock);
		PACKET_NUM_CASE(D6sys_tmback);
		PACKET_NUM_CASE(D6sys_multiops);
		PACKET_NUM_CASE(D6sys_itemup);
		PACKET_NUM_CASE(D6sys_statmsg);
		PACKET_NUM_CASE(D6sys_powerattack);
		PACKET_NUM_CASE(D6sys_reqloot);
		PACKET_NUM_CASE(D6sys_reqlootack);
		PACKET_NUM_CASE(D6sys_lootstatus);
		PACKET_NUM_CASE(D6sys_pcmoney);
		PACKET_NUM_CASE(D6sys_target);
		PACKET_NUM_CASE(D6sys_pcattr);
		PACKET_NUM_CASE(D6sys_monremake);
		PACKET_NUM_CASE(D6sys_vulnerable);
		PACKET_NUM_CASE(D6sys_moventpack);
		PACKET_NUM_CASE(D6sys_maptag);
		PACKET_NUM_CASE(D6sys_terrmapblock);
		PACKET_NUM_CASE(D6sys_terrmapbdone);
		PACKET_NUM_CASE(D6sys_tmbobjpack);
		PACKET_NUM_CASE(D6sys_tmbparty);
		PACKET_NUM_CASE(D6sys_mploadpos);
	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum EIR_Type : int16_t
{
	EIR_Default = 0,	// has no mods
	EIR_Trash = 1,		// has no mods

	EIR_Common = 2,
	EIR_Green = 3,
	EIR_Blue = 4,
	EIR_Violet = 5,
};

enum EIRMod_Common : int16_t
{
	EIRMC_Default = 0,

	EIRMC_Plus_1 = 1,
	EIRMC_Plus_2 = 2,
	EIRMC_Plus_3 = 3,
	EIRMC_Plus_4 = 4,
	EIRMC_Plus_5 = 5,
	EIRMC_Plus_6 = 6,
	EIRMC_Plus_7 = 7,
	EIRMC_Plus_8 = 8,
	EIRMC_Plus_9 = 9,
	EIRMC_Plus_10 = 10,
};

enum EIRMod_Green : int16_t
{
	EIRMG_Default = 0,

	EIRMG_Mods_1 = 1,
	EIRMG_Mods_2 = 2,
	EIRMG_Mods_3 = 3,
	EIRMG_Mods_4 = 4,
	EIRMG_Mods_5 = 5,

	EIRMG_Imbued = 6,
	EIRMG_Empowered = 7,
	EIRMG_Enchanted = 8,
	EIRMG_Exceptional = 9,
};

enum EIRMod_Blue : int16_t
{
	EIRMB_Default = 0,

	EIRMB_Mods_1 = 1,
	EIRMB_Mods_2 = 2,

	EIRMB_Superior_1 = 3,
	EIRMB_Superior_2 = 4,
	EIRMB_Superior_3 = 5,

	EIRMB_Brilliant = 6,
	EIRMB_Radiant = 7,
	EIRMB_Dazziling = 8,
	EIRMB_Supreme = 9,
};

enum EIRMod_Violet : int16_t
{
	EIRMV_Default = 0,

	EIRMV_Mods_1 = 1,
	EIRMV_Mods_2 = 2,
	EIRMV_Mods_3 = 3,
	EIRMV_Mods_4 = 4,
	EIRMV_Mods_5 = 5,

	EIRMV_Blessed = 6,
	EIRMV_Devout = 7,
	EIRMV_Divine = 8,
	EIRMV_Elite = 9,
};

struct dl_item_rarity_t
{
	int16_t type;
	int16_t mod;

	EIR_Type& GetType()
	{
		return reinterpret_cast<EIR_Type&>(type);
	}

	/*
		EIRMod_Common
		EIRMod_Green
		EIRMod_Blue
		EIRMod_Violet
	*/
	template<typename T>
	T& GetModAs()
	{
		return reinterpret_cast<T&>(mod);
	}

	std::string FormatRarity()
	{
		std::string buf;

		const auto& eType = GetType();
		switch (eType)
		{
		default:
		case EIR_Default:
		{
			buf = "[Default]";
			// nothing more
			break;
		}
		case EIR_Trash:
		{
			buf = "[Trash]";
			// nothing more
			break;
		}
		case EIR_Common:
		{
			buf = "[Common]";

			const auto& eMod = GetModAs<EIRMod_Common>();
			switch (eMod)
			{
			case EIRMC_Default:
				// nothing more
				break;

			default:
				// other +values
				buf += " +" + std::to_string(mod);
				break;
			}
			break;
		}
		case EIR_Green:
		{
			buf = "[Green]";

			const auto& eMod = GetModAs<EIRMod_Green>();
			switch (eMod)
			{
			default:
				// nothing more
				break;

			case EIRMG_Mods_1:
			case EIRMG_Mods_2:
			case EIRMG_Mods_3:
			case EIRMG_Mods_4:
			case EIRMG_Mods_5:
				buf += " [Mods]";
				break;

			case EIRMG_Imbued:
				buf += " [Imbued]";
				break;

			case EIRMG_Empowered:
				buf += " [Empowered]";
				break;

			case EIRMG_Enchanted:
				buf += " [Enchanted]";
				break;

			case EIRMG_Exceptional:
				buf += " [Exceptional]";
				break;
			}
			break;
		}
		case EIR_Blue:
		{
			buf = "[Blue]";

			const auto& eMod = GetModAs<EIRMod_Blue>();
			switch (eMod)
			{
			default:
				// nothing more
				break;

			case EIRMB_Mods_1:
			case EIRMB_Mods_2:
				buf += " [Mods]";
				break;

			case EIRMB_Superior_1:
			case EIRMB_Superior_2:
			case EIRMB_Superior_3:
				buf += " [Superior]";
				break;

			case EIRMB_Brilliant:
				buf += " [Brilliant]";
				break;

			case EIRMB_Radiant:
				buf += " [Radiant]";
				break;

			case EIRMB_Dazziling:
				buf += " [Dazziling]";
				break;

			case EIRMB_Supreme:
				buf += " [Supreme]";
				break;
			}
			break;
		}
		case EIR_Violet:
		{
			buf = "[Violet]";

			const auto& eMod = GetModAs<EIRMod_Violet>();
			switch (eMod)
			{
			default:
				// nothing more
				break;

			case EIRMV_Mods_1:
			case EIRMV_Mods_2:
			case EIRMV_Mods_3:
			case EIRMV_Mods_4:
			case EIRMV_Mods_5:
				buf += " [Mods]";
				break;

			case EIRMV_Blessed:
				buf += " [Blessed]";
				break;

			case EIRMV_Devout:
				buf += " [Devout]";
				break;

			case EIRMV_Divine:
				buf += " [Divine]";
				break;

			case EIRMV_Elite:
				buf += " [Elite]";
				break;
			}
			break;
		}
		}

		return buf;
	}
};

enum EIM_Type : int16_t
{
	EIM_None = 0,
	EIM_Strength = 1,
	EIM_Intellect = 2,
	EIM_Dexterity = 3,	// critical strike, parry
	EIM_Agility = 4,	// attack speed, dodge
	EIM_Vitality = 5,
	EIM_Honor = 6,
	EIM_Armor = 7,
	EIM_Parry = 8,
	EIM_Strike = 9,
	EIM_Critical = 10,
	EIM_Haste = 11,
	EIM_Damage = 12,
	EIM_Power = 13,
	EIM_Influence = 14,
	EIM_ResistMagic = 15,
	EIM_ResistFire = 16,
	EIM_ResistIce = 17,
	EIM_ResistPoison = 18,
	EIM_ResistPetrify = 19,
	EIM_ResistGas = 20,
};

static const std::string EIM_Type_Str(EIM_Type num)
{
#define PACKET_NUM_CASE(name) case EIM_Type::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(EIM_None);
		PACKET_NUM_CASE(EIM_Strength);
		PACKET_NUM_CASE(EIM_Intellect);
		PACKET_NUM_CASE(EIM_Dexterity);
		PACKET_NUM_CASE(EIM_Agility);
		PACKET_NUM_CASE(EIM_Vitality);
		PACKET_NUM_CASE(EIM_Honor);
		PACKET_NUM_CASE(EIM_Armor);
		PACKET_NUM_CASE(EIM_Parry);
		PACKET_NUM_CASE(EIM_Strike);
		PACKET_NUM_CASE(EIM_Critical);
		PACKET_NUM_CASE(EIM_Haste);
		PACKET_NUM_CASE(EIM_Damage);
		PACKET_NUM_CASE(EIM_Power);
		PACKET_NUM_CASE(EIM_Influence);
		PACKET_NUM_CASE(EIM_ResistMagic);
		PACKET_NUM_CASE(EIM_ResistFire);
		PACKET_NUM_CASE(EIM_ResistIce);
		PACKET_NUM_CASE(EIM_ResistPoison);
		PACKET_NUM_CASE(EIM_ResistPetrify);
		PACKET_NUM_CASE(EIM_ResistGas);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

#pragma pack(push, 1)

struct rnd_t
{
	int16_t from_itemIdx;
	int16_t to_itemIdx;
};

struct trlist_t
{
	rnd_t rnd[15];
};
static_assert(sizeof(trlist_t) == 60);

struct dl_trtable_params_t
{
	int16_t diceCount;
	int16_t param2;
	int16_t diceSides;
	int16_t param4;
	int16_t baseSum;
	int16_t param6;
	int16_t primaryThreshold;
	int16_t secondaryThreshold;
};

enum EDropType : int16_t
{
	eDT_Invalid = 0,
	eDT_RollDice1 = 1,
	eDT_RollDice2 = 2,
	eDT_TrList = 3,
};

static const std::string EDropType_Str(EDropType num)
{
#define PACKET_NUM_CASE(name) case EDropType::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(eDT_Invalid);
		PACKET_NUM_CASE(eDT_RollDice1);
		PACKET_NUM_CASE(eDT_RollDice2);
		PACKET_NUM_CASE(eDT_TrList);
	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

struct dl_trtable_entry_t
{
	EDropType dropType; // [1;2] - use RollDice, [3] - use ChooseItemFromTreasureList
	int16_t chance;
	dl_trtable_params_t params;
};

struct dl_trtable_t
{
	dl_trtable_entry_t entries[10];
};
static_assert(sizeof(dl_trtable_t) == 200);

struct dl_trap_t
{
	int id;
	int unk1;
	int type;
	int16_t unk3;
	int16_t trTableId;
	int16_t unk5;
	int16_t unk6;
	int16_t unk7;
	int16_t unk8[3];
	int16_t unk9[4];
};
static_assert(sizeof(dl_trap_t) == 36);

struct dl_item_t
{
	int16_t itemIdx;
	int16_t flags;
	int32_t durability;
	int32_t count;
	float unk1;

	int32_t maxCooldown;
	int32_t curCooldown;

	dl_item_rarity_t rarity;
	EIM_Type mod_type[5];
	int16_t mod_value[5];

	// clear cool down
	__forceinline void ClearCD()
	{
		if (maxCooldown > 0 && curCooldown > 0)
			curCooldown = 0;
	}

	std::vector<std::string> FormatMods()
	{
		std::vector<std::string> buf;

		for (int i = 0; i < 5; i++)
		{
			if (mod_type[i] > 0)
			{
				buf.emplace_back(std::format("[{}] {}: {}", i, EIM_Type_Str(mod_type[i]), mod_value[i]));
			}
		}

		return buf;
	}
};
static_assert(sizeof(dl_item_t) == 0x30); // 48

using EItemFlag = uint32_t; // EIF_
#define EIF(a,b) static constexpr EItemFlag EIF ## _ ## a = b

EIF(IsKey, 1 << 0);
EIF(IsArmor, 1 << 2);
EIF(Unk4, 1 << 3);
EIF(IsPotion, 1 << 4);

EIF(Unk1, 1 << 6);
EIF(Unk2, 1 << 9);
EIF(Unk5, 1 << 11);
EIF(Unk6, 1 << 13);
EIF(Unk7, 1 << 15);
EIF(Unk10, 1 << 16);
EIF(Unk8, 1 << 18);
EIF(Unk9, 1 << 21);
EIF(IsChainArmor, 1 << 26); // 0x4000000 | 67108864

EIF(IsSpear, EIF_Unk1 | EIF_Unk2 | EIF_IsChainArmor);
EIF(IsShadowStone, EIF_IsKey | EIF_Unk2 | EIF_Unk8 | EIF_Unk9); // 0x240201 | 2359809
EIF(IsLongSword, EIF_Unk2 | EIF_IsChainArmor); // 0x4000200 | 67109376
EIF(IsGiant, EIF_Unk1 | EIF_Unk2); // 0x240 | 576
EIF(IsStuff, EIF_Unk2 | EIF_Unk10); // 0x10200 | 66048
EIF(IsCantBeDropped, EIF_IsKey | EIF_Unk2 | EIF_Unk9); // 0x200201 | 2097665

#undef EIF

enum class EIF_Type : int16_t
{
	// CRYSTAL OF LIFE: type[0] subtype[0]
	// FLAMING SKULL: type[0] subtype[0]
	// DISPEL SPIRIT BARRIER: type[0] subtype[0]
	// unnamed, etc, idk
	UnkCrystal = 0,

	Attack = 1,

	// 2 does not present for some point

	Arrow = 3,
	Armor = 4,
	Shield = 5,
	Key = 6,
	ArcaneMagic = 7,
	Potion = 8,
	MagicMissle = 9,
	Cataal = 10, // bat wing, rat tail, etc
	Rune = 11, // is not used in DL Steam Edition
	CelestialMagic = 12,

	Gold = 14,
	Letter = 15,

	Accessory = 17,

	Kit = 20, // lockpicks, repair kits
	NetherMagic = 21,
	RuneMagic = 22,
	ClassAbility = 23, // charge, flash, divine, sneak, rune charge
};

static const std::string EIF_Type_Str(EIF_Type num)
{
#define PACKET_NUM_CASE(name) case EIF_Type::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(UnkCrystal);
		PACKET_NUM_CASE(Attack);
		PACKET_NUM_CASE(Arrow);
		PACKET_NUM_CASE(Armor);
		PACKET_NUM_CASE(Shield);
		PACKET_NUM_CASE(Key);
		PACKET_NUM_CASE(ArcaneMagic);
		PACKET_NUM_CASE(Potion);
		PACKET_NUM_CASE(MagicMissle);
		PACKET_NUM_CASE(Cataal);
		PACKET_NUM_CASE(Rune);
		PACKET_NUM_CASE(CelestialMagic);
		PACKET_NUM_CASE(Gold);
		PACKET_NUM_CASE(Letter);
		PACKET_NUM_CASE(Accessory);
		PACKET_NUM_CASE(Kit);
		PACKET_NUM_CASE(NetherMagic);
		PACKET_NUM_CASE(RuneMagic);
		PACKET_NUM_CASE(ClassAbility);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Attack : int16_t
{
	Melee = 0, // swords, claws, bites
	Throwing = 1, // knifes, surikens
	Bow = 2,
	Staff = 3, // weapon crystals, sceptre, staff
};

static const std::string EIF_Attack_Str(EIF_Attack num)
{
#define PACKET_NUM_CASE(name) case EIF_Attack::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Melee);
		PACKET_NUM_CASE(Throwing);
		PACKET_NUM_CASE(Bow);
		PACKET_NUM_CASE(Staff);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Arrow : int16_t
{
	Default = 0, // always
};

static const std::string EIF_Arrow_Str(EIF_Arrow num)
{
#define PACKET_NUM_CASE(name) case EIF_Arrow::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Armor : int16_t
{
	Chest = 0,
	Pants = 1,

	// 2 not presented

	Arms = 3,
	Boots = 4,
	Helmet = 5,
	Hair = 6,
	Shoulders = 7,

	RightShoulder = 8,
	LeftShoulder = 9,
};

static const std::string EIF_Armor_Str(EIF_Armor num)
{
#define PACKET_NUM_CASE(name) case EIF_Armor::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Chest);
		PACKET_NUM_CASE(Pants);
		PACKET_NUM_CASE(Arms);
		PACKET_NUM_CASE(Boots);
		PACKET_NUM_CASE(Helmet);
		PACKET_NUM_CASE(Hair);
		PACKET_NUM_CASE(Shoulders);
		PACKET_NUM_CASE(RightShoulder);
		PACKET_NUM_CASE(LeftShoulder);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Shield : int16_t
{
	Default = 0, // always
};

static const std::string EIF_Shield_Str(EIF_Shield num)
{
#define PACKET_NUM_CASE(name) case EIF_Shield::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Key : int16_t
{
	Key = 0,
	Stone = 1,
};

static const std::string EIF_Key_Str(EIF_Key num)
{
#define PACKET_NUM_CASE(name) case EIF_Key::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Key);
		PACKET_NUM_CASE(Stone);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_ArcaneMagic : int16_t
{
	Default = 0,
};

static const std::string EIF_ArcaneMagic_Str(EIF_ArcaneMagic num)
{
#define PACKET_NUM_CASE(name) case EIF_ArcaneMagic::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Potion : int16_t
{
	Default = 0,
};

static const std::string EIF_Potion_Str(EIF_Potion num)
{
#define PACKET_NUM_CASE(name) case EIF_Potion::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_MagicMissle : int16_t
{
	Default = 0,
};

static const std::string EIF_MagicMissle_Str(EIF_MagicMissle num)
{
#define PACKET_NUM_CASE(name) case EIF_MagicMissle::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Cataal : int16_t
{
	BatWing = 0,
	RatTail = 1,
	WolfMane = 2,
	DeadmanHair = 3,
	SnakeSkin = 4,
	ScorpionTail = 5,
	RavenClaw = 6,
	MonkeyPaw = 7,
	VultureBeak = 8,
	BorlothHorn = 9,

	DragonTooth = 10,
	DemonHorn = 11,
	ShrunkenHead = 12,
	DriedHomonculous = 13,
	SpiderYolk = 14,
	PutridFlesh = 15,
	BloodNectre = 16,
	WyrmGangre = 17,
	MonsterEye = 18,
	BoneDust = 19,

	OchrePollen = 20,
	BrimstonePowder = 21,
	DiamondDust = 22,
	SoulStone = 23,
	GrinnichWeed = 24,
	OpheliaWort = 25,
	MordisVine = 26,
	BlackOrchid = 27,
	MandrakeRoot = 28,
};

static const std::string EIF_Cataal_Str(EIF_Cataal num)
{
#define PACKET_NUM_CASE(name) case EIF_Cataal::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(BatWing);
		PACKET_NUM_CASE(RatTail);
		PACKET_NUM_CASE(WolfMane);
		PACKET_NUM_CASE(DeadmanHair);
		PACKET_NUM_CASE(SnakeSkin);
		PACKET_NUM_CASE(ScorpionTail);
		PACKET_NUM_CASE(RavenClaw);
		PACKET_NUM_CASE(MonkeyPaw);
		PACKET_NUM_CASE(VultureBeak);
		PACKET_NUM_CASE(BorlothHorn);
		PACKET_NUM_CASE(DragonTooth);
		PACKET_NUM_CASE(DemonHorn);
		PACKET_NUM_CASE(ShrunkenHead);
		PACKET_NUM_CASE(DriedHomonculous);
		PACKET_NUM_CASE(SpiderYolk);
		PACKET_NUM_CASE(PutridFlesh);
		PACKET_NUM_CASE(BloodNectre);
		PACKET_NUM_CASE(WyrmGangre);
		PACKET_NUM_CASE(MonsterEye);
		PACKET_NUM_CASE(BoneDust);
		PACKET_NUM_CASE(OchrePollen);
		PACKET_NUM_CASE(BrimstonePowder);
		PACKET_NUM_CASE(DiamondDust);
		PACKET_NUM_CASE(SoulStone);
		PACKET_NUM_CASE(GrinnichWeed);
		PACKET_NUM_CASE(OpheliaWort);
		PACKET_NUM_CASE(MordisVine);
		PACKET_NUM_CASE(BlackOrchid);
		PACKET_NUM_CASE(MandrakeRoot);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Rune : int16_t
{
	Aenir = 0,
	Bruenguld = 1,
	Cythoen = 2,
	Demetos = 3,
	Erillae = 4,
	Feohn = 5,
	Geomynn = 6,
	Haentir = 7,
	Isildorn = 8,
	Jyrgaed = 9,
};

static const std::string EIF_Rune_Str(EIF_Rune num)
{
#define PACKET_NUM_CASE(name) case EIF_Rune::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Aenir);
		PACKET_NUM_CASE(Bruenguld);
		PACKET_NUM_CASE(Cythoen);
		PACKET_NUM_CASE(Demetos);
		PACKET_NUM_CASE(Erillae);
		PACKET_NUM_CASE(Feohn);
		PACKET_NUM_CASE(Geomynn);
		PACKET_NUM_CASE(Haentir);
		PACKET_NUM_CASE(Isildorn);
		PACKET_NUM_CASE(Jyrgaed);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_CelestialMagic : int16_t
{
	Default = 0,

	Special = 2, // DIVINE STRIKE only
};

static const std::string EIF_CelestialMagic_Str(EIF_CelestialMagic num)
{
#define PACKET_NUM_CASE(name) case EIF_CelestialMagic::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);
		PACKET_NUM_CASE(Special);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Gold : int16_t
{
	Default = 0,
};

static const std::string EIF_Gold_Str(EIF_Gold num)
{
#define PACKET_NUM_CASE(name) case EIF_Gold::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Letter : int16_t
{
	Default = 0,
};

static const std::string EIF_Letter_Str(EIF_Letter num)
{
#define PACKET_NUM_CASE(name) case EIF_Letter::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Accessory : int16_t
{
	GaldrynsHorn = 0,
	Ring = 1,
	Belt = 2,
	Charm = 3,
	Brace = 4,
};

static const std::string EIF_Accessory_Str(EIF_Accessory num)
{
#define PACKET_NUM_CASE(name) case EIF_Accessory::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(GaldrynsHorn);
		PACKET_NUM_CASE(Ring);
		PACKET_NUM_CASE(Belt);
		PACKET_NUM_CASE(Charm);
		PACKET_NUM_CASE(Brace);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_Kit : int16_t
{
	Lockpick = 0,
	Repair = 1,
};

static const std::string EIF_Kit_Str(EIF_Kit num)
{
#define PACKET_NUM_CASE(name) case EIF_Kit::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Lockpick);
		PACKET_NUM_CASE(Repair);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_NetherMagic : int16_t
{
	Default = 0,
};

static const std::string EIF_NetherMagic_Str(EIF_NetherMagic num)
{
#define PACKET_NUM_CASE(name) case EIF_NetherMagic::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_RuneMagic : int16_t
{
	Default = 0,
	Armor = 1,
	Ultimate = 2,
};

static const std::string EIF_RuneMagic_Str(EIF_RuneMagic num)
{
#define PACKET_NUM_CASE(name) case EIF_RuneMagic::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);
		PACKET_NUM_CASE(Armor);
		PACKET_NUM_CASE(Ultimate);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum class EIF_ClassAbility : int16_t
{
	Default = 0,
};

static const std::string EIF_ClassAbility_Str(EIF_ClassAbility num)
{
#define PACKET_NUM_CASE(name) case EIF_ClassAbility::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(Default);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

// Item::Something ida
struct dl_item_info_t
{
	char name[32];
	EIF_Type curType;
	int16_t subType;
	int16_t unk3;
	int16_t unk4;
	int16_t type;
	char unkType[2];
	int16_t model_num;
	int16_t unk6;
	int16_t unk7;
	int16_t unk8;
	int32_t unk9;
	int16_t spellId;
	int16_t durability;
	int32_t price;
	int32_t unk14;
	int16_t unk15;
	int16_t unk16;
	int16_t skillType;
	uint32_t flags;
	int32_t unk18;
	int32_t unk19;
	int32_t unk20;
	int32_t unk21;
	int32_t duration;
	int16_t magicStrikeType;
	int16_t magicChargeType;
	int16_t unk24;
	int32_t count;
	int16_t unk27;
	int16_t unk28;
	int16_t iArmor;
	int16_t unk30;
	int16_t unk31;
	int16_t unk32;
	int16_t unk33;
	int16_t unk34;
	int16_t unk35;
	int16_t unk36;
	int32_t dmg1;
	int32_t dmg2;
	int32_t dmg3;
	char pad1[6];
	int16_t statusEffectType;
	int unk999;
	char pad2[14];
	char pad3[6];
	int16_t iStatFromItem1;
	int16_t iStatFromItem2;
	int16_t iParry;
	int16_t iStatFromItem4;
	int16_t iStrike;
	int16_t iStatFromItem6;
	char pad4[10];
	char pad5[12];
	dl_item_rarity_t rarity;
	EIM_Type mod_type[5];
	int16_t mod_value[5];
	int16_t pad6[3];

	EIF_Type& GetType()
	{
		return curType;
	}

	template<typename T>
	T GetSubTypeAs()
	{
		return static_cast<T>(subType);
	}

	std::string GetTypeStr() const
	{
		return EIF_Type_Str(curType);
	}

	std::string GetTypeData()
	{
#define SwitchType(name) case EIF_Type::name: return CB_COMBINE(CB_COMBINE(EIF_,name),_Str)(static_cast<CB_COMBINE(EIF_,name)>(subType));

		const auto& eType = GetType();
		switch (eType)
		{
		default:
		case EIF_Type::UnkCrystal:
			return std::to_string(subType);
			break;

			SwitchType(Attack);
			SwitchType(Arrow);
			SwitchType(Armor);
			SwitchType(Shield);
			SwitchType(Key);
			SwitchType(ArcaneMagic);
			SwitchType(Potion);
			SwitchType(MagicMissle);
			SwitchType(Cataal);
			SwitchType(Rune);
			SwitchType(CelestialMagic);
			SwitchType(Gold);
			SwitchType(Letter);
			SwitchType(Accessory);
			SwitchType(Kit);
			SwitchType(NetherMagic);
			SwitchType(RuneMagic);
			SwitchType(ClassAbility);
		}

#undef SwitchType
	}
};
static_assert(sizeof(dl_item_info_t) == 0xEC); // 236
#pragma pack(pop)