# Initialize Forgot Engine

## 就職作品
  個人で制作したエンジンと制作中のゲームのリポジトリです。

## 製作期間
エンジン : 10ヶ月
アプリケーション : 5ヶ月

## 2D/3D描画エンジン

  DirectX12を用いた自作エンジンです。  
  コンポーネント指向を組み込みエディタとしても利用できます。

### 基本機能
- コンポーネント
- サウンドの読み込み(waveのみの対応)
- シーンデータの書き込み及び読み込み

### エディタ機能
  起動中にImgui上で行うことができる操作
- モデル読み込み
- テクスチャ読み込み
- オブジェクトの追加
- スプライトの追加
- パーティクルの追加
- 色の変更
- マテリアルの変更
- テクスチャの変更
- コライダーの追加
- ポストエフェクトの調整
- ライトの設置
など

## 制作環境
- DirectX12
- Windows SDK 10.0.22000.0

## 使用外部ライブラリ
- [Assimp](https://github.com/assimp/assimp)
  - モデルの読み込みに使用
- [DirectXTex](https://github.com/microsoft/DirectXTex)
  - テクスチャの読み込みに使用
- [Dear ImGui](https://github.com/ocornut/imgui)
  - エディタ画面のGUIに使用
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
  - エディタ画面の3dギズモに使用
- [JSON for Modern C++](https://github.com/nlohmann/json)
  - シーンデータの書き込み及び読み込みに使用
- [strconv](https://github.com/javacommons/strconv)
  - 文字列の変換に使用

## 作成した実行ファイルについて
Visual Studioやランタイムライブラリのない環境での動作確認済み
