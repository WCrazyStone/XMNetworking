//
//  XMURLSessionTaskDelegate.h
//  XMNetworking
//
//  Created by Crazy on 15/9/6.
//  Copyright © 2015年 Crazy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMHTTPClient.h"

@class XMURLSessionTaskDelegate;

/**
 *  An block responsible for transforming the result of a task (response + data) to the resulting
 *  object to be used as the <body> parameter of BARequest. Uses of this block include transforming
 *  raw JSON data to NSDictionary/NSArray, or transforming data to images etc.
 *
 *  @param response The NSURLResponse of the task.
 *  @param data     The raw data returned for the task.
 *  @param delegate The delegate responsible for handling the processing of this task.
 *
 *  @return Any object representing the result object of the request.
 */
typedef id (^XMHTTPResponseProcessBlock) (NSURLResponse *response, NSData *data, XMURLSessionTaskDelegate *delegate);

@interface XMURLSessionTaskDelegate : NSObject

/**
 * The download location of a NSURLSessionDownloadTask. This will be set if the task:didDownloadToLocation:
 * method is called with a location.
 */
@property (nonatomic, copy, readonly) NSURL *downloadLocation;

/**
 *  If a client side error occurs that is not a network error (e.g. during response processing), this
 *  property can be set by calling task:didError: and providing a custom error. The effect of this
 *  will be that even if task:DidCompleteWithResponse:error: is called with a nil error, this error
 *  will be propagated to the completion block if set.
 */
@property (nonatomic, copy, readonly) NSError *error;

/**
 *  Initiallize a new task delegate.
 *
 *  @param request                 The original BARequest for the task.
 *
 *  @param responseProcessingQueue The queue on which the responseProcessBlock should be exectured.
 *
 *  @param progressBlock           A block to be called any time the task makes progress. Triggered as a result of
 *                                 taskDidUpdateProgress: being called. This is optional and can be nil.
 *  @param responseProcessBlock    A block to transform the resulting NSData to a more suitable response object of any type.
 *  @param completionBlock         A block that will be called once and only once as a result of
 *                                 task:didCompleteWithResponse:error: being called.
 *
 *  @return A new task delegate.
 */
- (instancetype)initWithRequest:(XMRequest *)request
        responseProcessingQueue:(dispatch_queue_t)responseProcessingQueue
                  progressBlock:(XMRequestProgressBlock)progressBlock
           responseProcessBlock:(XMHTTPResponseProcessBlock)responseProcessBlock
                completionBlock:(XMRequestCompletionBlock)completionBlock;

/**
 *  To be called as a result of -[NSURLSessionDataDelegate URLSessiondataTask:didReceiveData:]. If this is called
 *  multiple time the data will be appended to the previous value.
 *
 *  @param task The task that received new data.
 *  @param data The chunk of data received.
 */
- (void)task:(NSURLSessionTask *)task didReceiveData:(NSData *)data;

/**
 *  To be called as a result of [NSURLSessionDownloadDelegate URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesExpectedToWrite:]
 *
 *  @param task The task that made progress.
 */
- (void)taskDidUpdateProgress:(NSURLSessionTask *)task;

/**
 *  To be called as a result of -[NSURLSessionTaskDelegate URLSession:task:didCompleteWithError:].
 *
 *  @param task     The task that completed.
 *  @param error    Any error that occurred.
 */
- (void)task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error;

/**
 *  To be called as a result of [NSURLSessionDownloadDelegate URLSession:downloadTask:didFinishDownloadingToURL:]
 *
 *  @param task     The task.
 *  @param location The file location where the file was downloaded to.
 */
- (void)task:(NSURLSessionTask *)task didFinishDownloadingToURL:(NSURL *)location;

/**
 *  If any error occurrs in the processing before -task:didCompleteWithResponse:error:, call this method
 *  to indicate this. The will cause that error to be delivered to the completion block even if the
 *  The error of -task:didCompleteWithResponse:error: was nil.
 *
 *  @param task  The task.
 *  @param error The error that occurred.
 */
- (void)task:(NSURLSessionTask *)task didError:(NSError *)error;

@end
