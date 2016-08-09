<?php

/**
 * @file es_retrieve_index.inc
 * Elastic Search Index Doc Name
 */
function _es_get_index_name($domain_id) {

  $index_name = '';

  switch ($domain_id) {

    case '1':
      # www.fccnn.com
      $index_name = '';
      break;

    case '2':
      # www.inforum.com
      $index_name = '';
      break;

    case '3':
      # www.agweek.com
      $index_name = '';
      break;

    case '4':
      # www.dglobe.com
      $index_name = '';
      break;

    case '5':
      # www.dl-online.com
      $index_name = '';
      break;

    case '6':
      # www.duluthbudgeteer.com
      $index_name = '';
      break;

    case '7':
      # www.duluthnewstribune.com
      $index_name = '';
      break;

    case '8':
      # www.echopress.com
      $index_name = '';
      break;

    case '9':
      # www.perhamfocus.com
      $index_name = '';
      break;

    case '10':
      # www.farmingtonindependent.com
      $index_name = '';
      break;

    case '11':
      # www.grandforks.com
      $index_name = '';
      break;

    case '12':
      # www.grandforksherald.com
      $index_name = '';
      break;

    case '13':
      # www.hastingsstargazette.com
      $index_name = '';
      break;

    case '14':
      # www.hudsonstarobserver.com
      $index_name = '';
      break;

    case '15':
      # www.inforum.com
      $index_name = 'inforum';
      break;

    case '16':
      # www.jamestownsun.com
      $index_name = '';
      break;

    case '17':
      # www.mitchelladvisor.com
      $index_name = '';
      break;

    case '18':
      # www.mitchellrepublic.com
      $index_name = '';
      break;

    case '19':
      # www.morrissuntribune.com
      $index_name = '';
      break;

    case '20':
      # www.newrichmond-news.com
      $index_name = '';
      break;

    case '21':
      # www.northlandoutdoors.com
      $index_name = '';
      break;

    case '22':
      # www.parkrapidsenterprise.com
      $index_name = '';
      break;

    case '23':
      # www.piercecountyherald.com
      $index_name = '';
      break;

    case '24':
      # www.pinejournal.com
      $index_name = '';
      break;

    case '25':
      # www.prairiebusinessmagazine.com
      $index_name = '';
      break;

    case '26':
      # www.republican-eagle.com
      $index_name = '';
      break;

    case '27':
      # www.riverfallsjournal.com
      $index_name = '';
      break;

    case '28':
      # www.rivertowns.net
      $index_name = '';
      break;

    case '29':
      # www.rosemounttownpages.com
      $index_name = '';
      break;

    case '30':
      # www.superiortelegram.com
      $index_name = '';
      break;

    case '31':
      # www.swcbulletin.com
      $index_name = '';
      break;

    case '32':
      # www.thedickinsonpress.com
      $index_name = '';
      break;

    case '33':
      # www.theosakisreview.com
      $index_name = '';
      break;

    case '34':
      # www.bakkentoday.com
      $index_name = '';
      break;

    case '35':
      # www.twoharborsmn.com/www.lcnewschronicle.com
      $index_name = '';
      break;

    case '36':
      # www.wadenapj.com
      $index_name = '';
      break;

    case '37':
      # www.wctrib.com
      $index_name = '';
      break;

    case '38':
      # www.wday.com
      $index_name = '';
      break;

    case '39':
      # www.wdaz.com
      $index_name = '';
      break;

    case '40':
      # www.westfargopioneer.com
      $index_name = '';
      break;

    case '41':
      # www.woodburybulletin.com
      $index_name = '';
      break;

    case '42':
      # www.brainerddispatch.com
      $index_name = '';
      break;

    case '43':
      # www.pineandlakes.com
      $index_name = '';
      break;

    default:
      # All sites search
      $index_name = 'node';
      break;
  }

  return $index_name;
}
