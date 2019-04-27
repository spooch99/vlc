/*****************************************************************************
 * VLCPlaylistController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#import <Foundation/Foundation.h>
#import <vlc_playlist.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCPlaylistModel;
@class VLCPlaylistDataSource;
@class VLCPlayerController;
@class VLCPlaylistExportModuleDescription;

extern NSString *VLCPlaybackOrderChanged;
extern NSString *VLCPlaybackRepeatChanged;
extern NSString *VLCPlaybackHasPreviousChanged;
extern NSString *VLCPlaybackHasNextChanged;
extern NSString *VLCPlaylistCurrentItemChanged;
extern NSString *VLCPlaylistItemsAdded;
extern NSString *VLCPlaylistItemsRemoved;

@interface VLCPlaylistController : NSObject

- (instancetype)initWithPlaylist:(vlc_playlist_t *)playlist;
- (void)deinitialize;

/**
 * The vlc core playlist controlled by the instance of this class
 * @note You DO SOMETHING WRONG if you need to access this!
 */
@property (readonly) vlc_playlist_t *p_playlist;

/**
 * The playlist model caching the contents of the playlist controlled by
 * the instance of this class.
 */
@property (readonly) VLCPlaylistModel *playlistModel;

/**
 * The datasource instance used to actually display the playlist.
 */
@property (readwrite, assign) VLCPlaylistDataSource *playlistDataSource;

/**
 * The player instance associated with the playlist
 */
@property (readonly) VLCPlayerController *playerController;

/**
 * Index of the current playlist item
 @return index of the current playlist index or -1 if none
 @warning just because the current index is valid does not imply that it is playing!
 */
@property (readonly) size_t currentPlaylistIndex;

/**
 * input of the currently playing item
 @return returns the input item for the currently playing playlist item
 @note the receiver is responsible for releasing the input item
 @note Subscribe to the VLCPlaylistCurrentItemChanged notification to be notified about changes
 */
@property (readonly, nullable) input_item_t *currentlyPlayingInputItem;

/**
 * indicates whether there is a previous item in the list the user could go back to
 * @note Subscribe to the VLCPlaybackHasPreviousChanged notification to be notified about changes
 */
@property (readonly) BOOL hasPreviousPlaylistItem;

/**
 * indicates whether there is a next item in the list the user could move on to
 * @note Subscribe to the VLCPlaybackHasNextChanged notification to be notified about changes
 */
@property (readonly) BOOL hasNextPlaylistItem;

/**
 * sets and gets the playback repeat mode according to the enum defined in the core
 * @note Subscribe to the VLCPlaybackRepeatChanged notification to be notified about changes
 */
@property (readwrite, nonatomic) enum vlc_playlist_playback_repeat playbackRepeat;

/**
 * sets and gets the playback order according to the enum defined in the core
 * @note Subscribe to the VLCPlaybackOrderChanged notification to be notified about changes
 */
@property (readwrite, nonatomic) enum vlc_playlist_playback_order playbackOrder;

/**
 * Simplified version to add new items to the end of the current playlist
 * @param array array of items. Each item is a Dictionary with meta info.
 */
- (void)addPlaylistItems:(NSArray*)array;

/**
 * Add new items to the playlist, at specified index.
 * @param itemArray array of items. Each item is a Dictionary with meta info.
 * @param insertionIndex index for new items, -1 for appending at end
 * @param startPlayback starts playback of first item if true
 */
- (void)addPlaylistItems:(NSArray*)itemArray
              atPosition:(size_t)insertionIndex
           startPlayback:(BOOL)startPlayback;

/**
 * Add new item to the playlist if you already have an input item
 * @param p_inputItem the input item you already from somewhere
 * @param insertionIndex index for new item, -1 for appending at end
 * @param startPlayback starts playback of the item if true
 * @return returns VLC_SUCCESS or an error
 */
- (int)addInputItem:(input_item_t *)p_inputItem
         atPosition:(size_t)insertionIndex
      startPlayback:(BOOL)startPlayback;

/**
 * Remove the item at the given index (if any)
 * @param index the index to remove
 */
- (void)removeItemAtIndex:(size_t)index;

/**
 * Clear the entire playlist
 */
- (void)clearPlaylist;

/**
 * Start the playlist
 * @return Returns VLC_SUCCESS on success.
 */
- (int)startPlaylist;

/**
 * Play the previous item in the list (if any)
 * @return Returns VLC_SUCCESS on success.
 */
- (int)playPreviousItem;

/**
 * Play the item at the given index (if any)
 * @param index the index to play
 * @return Returns VLC_SUCCESS on success.
 */
- (int)playItemAtIndex:(size_t)index;

/**
 * Play the previous item in the list (if any)
 * @return Returns VLC_SUCCESS on success.
 */
- (int)playNextItem;

/**
 * Stop playback of the playlist and destroy all facilities
 */
- (void)stopPlayback;

/**
 * Pause playback of the playlist while keeping all facilities
 */
- (void)pausePlayback;

/**
 * Resume playback of the playlist if it was paused
 */
- (void)resumePlayback;

/**
 * returns an array of module descriptions available for export a playlist. Content may vary
 */
@property (readonly) NSArray <VLCPlaylistExportModuleDescription *> *availablePlaylistExportModules;

/**
 * exports the playlist owned by the controller to a given file using the provided module
 * @param path the file path to store the file in
 * @param exportModule the VLCPlaylistExportModuleDescription for the respective export module
 * @return VLC_SUCCESS or an error
 */
- (int)exportPlaylistToPath:(NSString *)path exportModule:(VLCPlaylistExportModuleDescription *)exportModule;

@end

@interface VLCPlaylistExportModuleDescription : NSObject

@property (readwrite, retain) NSString *humanReadableName;
@property (readwrite, retain) NSString *fileExtension;
@property (readwrite, retain) NSString *moduleName;

@end

NS_ASSUME_NONNULL_END
