//
//  XATabWindow.h
//  XChatAqua
//
//  Created by Jeong YunWon on 12. 6. 14..
//  Copyright (c) youknowone.org All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class SGTabView;

@interface XATabWindow : NSWindow<XAEventChain>

@property(nonatomic, readonly) SGTabView *tabView;

@end

@protocol XATabWindowDelegate<NSObject>

- (void)windowCloseTab:(XATabWindow *)window;

@end
