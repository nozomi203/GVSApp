# GVSApp

① GvsApp/SeeeduinoCCC以下のプログラムを電気刺激装置のマイコンに書き込む（プログラム内の変数g_Chをデバイスごとに書き換え）

② GvsApp/build/GVSApp/GVS.exeを実行．「Add Device」を押し，表示されたボックス内のIDを1.で書き換えたg_Chに合わせる．また，PortNameもデバイスが接続されているポートに合わせて書き換える．Current,Frequency,Duration,WaveFormをそれぞれ調整したらStartで刺激開始．

## 要素説明
channel: デバイスを識別する数字．0-7まで設定．
portname: 接続するポートの名前．デバイスマネージャーで調べる．
current: 電流値．0（刺激なし）- 4095（デバイスの出せる最大値）の間で設定．
frequency: 周波数．0-1023(Hz)の間で設定．
waveform: 刺激波形．square_bipole→両極矩形波．square_positive→正の片側矩形波．square_negative→負の片側矩形波．direct_positive→正の直流．direct_negative→負の直流
duration: 刺激の継続時間．0-180(秒)の間で設定．
transition_duration: 刺激強度の最大値に達するまでの時間．0-15の間で設定．
transition_form: 刺激強度の最大値に達するまでの遷移関数．constant→遷移なし．linear→線形に増加．smooth→スムーズに接続（t*t*(3 - 2 * t）．

## 注意点
・ポート名が間違っている場合はその旨が表示されるので正しいポート名に直す．
・ポート名が合っていてもデバイスからの応答がない場合，ボックスの左下に「No device」と表示される．この場合はTeratarm等を駆使してデバッグする．
・予期せぬバグによって電流が止まらなくなる可能性は十分にありうるので，そうなったらデバイスの給電を抜いて強制的に停止させる．

---

# macで使いたい場合
Arduino IDEでSeeduinoCCC以下のプログラムを書くところまではWindows(上記)と同様で大丈夫だけど，QtCreatorのバージョンを揃えたりPortNameの調べ方がちょっと癖あるので注意する．
事前準備として，Xcodeをダウンロードしておくこと．QtCreatorも大概だけどこちらもめちゃ重いので時間がかかる．

※SeeduinoXIAOに書き込む方法がわからない時は[公式](https://wiki.seeedstudio.com/jp/Seeeduino-XIAO/)を参考にする．

## 以下手順
1. QtCreatorのインストローラを[HP](https://www.qt.io/download-open-source)からダウンロードする
2. Installするverを選択する時に，Archiveにチェックを入れてFilterをかける
3. ***Qt/5.12.10***にチェックを入れて付属のモジュールもとりあえず全てチェックしてインストール(時間かかる)
4. GVS.proをダブルクリックしてQtCreatorで開く
5. configurationしてプロジェクトをインポートする
6. ターミナルを開いて，`ls -l /dev/tty.*`のコマンドを実行してUSBのポート番号を調べる
7. `tty.usbserial-hoge`をコピーしておく
8. GVS.appを左下の再生マークから実行する
9. `Add Devices`からChannelを指定し，PortNameに先程の`tty.usbserial-hoge`のttyをcuに置換して入力
10. No deviceがなくなるので，Startボタンを押して実行
