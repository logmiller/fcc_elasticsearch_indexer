<?php

/**
 * @file fcc_elasticsearch.subdomains.inc
 */
function _es_get_subdomain( $domain_id ) {

	$subdomain = '';

	switch ( $domain_id ) {

		case '1':
			$subdomain = 'www.fccnn.com';
			break;

		case '2':
			$subdomain = 'www.bemidjipioneer.com';
			break;

		case '3':
			$subdomain = 'www.agweek.com';
			break;

		case '4':
			$subdomain = 'www.dglobe.com';
			break;

		case '5':
			$subdomain = 'www.dl-online.com';
			break;

		case '6':
			$subdomain = 'www.duluthbudgeteer.com';
			break;

		case '7':
			$subdomain = 'www.duluthnewstribune.com';
			break;

		case '8':
			$subdomain = 'www.echopress.com';
			break;

		case '9':
			$subdomain = 'www.perhamfocus.com';
			break;

		case '10':
			$subdomain = 'www.farmingtonindependent.com';
			break;

		case '11':
			$subdomain = 'www.grandforks.com';
			break;

		case '12':
			$subdomain = 'www.grandforksherald.com';
			break;

		case '13':
			$subdomain = 'www.hastingsstargazette.com';
			break;

		case '14':
			$subdomain = 'www.hudsonstarobserver.com';
			break;

		case '15':
			$subdomain = 'www.inforum.com';
			break;

		case '16':
			$subdomain = 'www.jamestownsun.com';
			break;

		case '17':
			$subdomain = 'www.mitchelladvisor.com';
			break;

		case '18':
			$subdomain = 'www.mitchellrepublic.com';
			break;

		case '19':
			$subdomain = 'www.morrissuntribune.com';
			break;

		case '20':
			$subdomain = 'www.newrichmond-news.com';
			break;

		case '21':
			$subdomain = 'www.northlandoutdoors.com';
			break;

		case '22':
			$subdomain = 'www.parkrapidsenterprise.com';
			break;

		case '23':
			$subdomain = 'www.piercecountyherald.com';
			break;

		case '24':
			$subdomain = 'www.pinejournal.com';
			break;

		case '25':
			$subdomain = 'www.prairiebizmag.com';
			break;

		case '26':
			$subdomain = 'www.republican-eagle.com';
			break;

		case '27':
			$subdomain = 'www.riverfallsjournal.com';
			break;

		case '28':
			$subdomain = 'www.rivertowns.net';
			break;

		case '29':
			$subdomain = 'www.rosemounttownpages.com';
			break;

		case '30':
			$subdomain = 'www.superiortelegram.com';
			break;

		case '31':
			$subdomain = 'www.swcbulletin.com';
			break;

		case '32':
			$subdomain = 'www.thedickinsonpress.com';
			break;

		case '33':
			$subdomain = 'www.theosakisreview.com';
			break;

		case '34':
			$subdomain = 'www.bakkentoday.com';
			break;

		case '35':
			$subdomain = 'www.twoharborsmn.com';
			break;

		case '36':
			$subdomain = 'www.wadenapj.com';
			break;

		case '37':
			$subdomain = 'www.wctrib.com';
			break;

		case '38':
			$subdomain = 'www.wday.com';
			break;

		case '39':
			$subdomain = 'www.wdaz.com';
			break;

		case '40':
			$subdomain = 'www.westfargopioneer.com';
			break;

		case '41':
			$subdomain = 'www.woodburybulletin.com';
			break;

		case '42':
			$subdomain = 'www.brainerddispatch.com';
			break;

		case '43':
			$subdomain = 'www.pineandlakes.com';
			break;

		default:
			# All sites search
			break;
	}
        unset($domain_id); // unset domain_id
	return $subdomain;
}
